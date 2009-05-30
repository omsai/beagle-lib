/**
 * @file	beagle.h
 *
 * @brief This file documents the API as well as header for the Broad-platform Evolutionary Analysis General Likelihood Evaluator
 *
 * LONG COMMENTS HERE
 *
 * OVERVIEW:
 *
 * KEY ELEMENTS:  INSTANCE, BUFFER, etc.
 *
 * @author Likelihood API Working Group
 *
 */

#ifndef __beagle__
#define __beagle__

enum BeagleReturnCodes {
	NO_ERROR = 0,
	OUT_OF_MEMORY_ERROR = 1,
	GENERAL_ERROR
};

enum BeagleFlags {
	DOUBLE	=1<<0, /**< Request/require double precision computation */
	SINGLE	=1<<1, /**< same */
	ASYNCH	=1<<2,
	SYNCH	=1<<3,
	CPU		=1<<16,
	GPU		=1<<17,
	FPGA	=1<<18,
	SSE		=1<<19,
	CELL	=1<<20
};


/**
 * @brief Structure includes information about a specific instance
 *
 * LONG DESCRIPTION
 *
 */
typedef struct {
	int resourceNumber; /**< Resource upon which instance is running */
	int flags; 			/**< Bit-flags that characterize this instance's resource */
} InstanceDetails;

typedef struct {
	char* name;
	long flag;
} Resource;

typedef struct {
	Resource* list;
	int length;
} ResourceList;


/**
 * @brief
 *
 * LONG DESCRIPTION
 *
 * @return A list of resources available to the library as a ResourceList array
 */
// returns a list of computing resources
ResourceList* getResourceList();

/**
 * Create a single instance
 * This can be called multiple times to create multiple data partition instances
 * each returning a unique identifier.
 *
 * nodeCount the number of nodes in the tree
 * tipCount the number of tips in the tree
 * stateCount the number of states
 * patternCount the number of site patterns
 *
 *
 *
 * @return the unique instance identifier (-1 if failed)
 *
 */
int createInstance(
			    int tipCount,				/**< Number of tip data elements (input) */
				int partialsBufferCount,	/**< Number of partials buffers to create (input) */
				int compactBufferCount,		/**< Number of compact state representation buffers to create (input) */
				int stateCount,				/**< Number of states in the continuous-time Markov chain (input) */
				int patternCount,			/**< Number of site patterns to be handled by the instance (input) */
				int eigenBufferCount,		/**< Number of rate matrix eigen-decomposition buffers to allocate (input) */
				int matrixBufferCount,		/**< Number of rate matrix buffers (input) */
				int* resourceList,			/**< List of potential resource on which this instance is allowed (input, NULL implies no restriction */
				int resourceCount,			/**< Length of resourceList list (input) */
				int preferenceFlags,		/**< Bit-flags indicating preferred implementation charactertistics, see BeagleFlags (input) */
				int requirementFlags		/**< Bit-flags indicating required implementation characteristics, see BeagleFlags (input) */
				);

// initialization of instance,  returnInfo can be null
int initializeInstance(
						int instance,		/**< Instance number to initialize (input) */
						InstanceDetails* returnInfo);


// finalize and dispose of memory allocation if needed
int finalize(int *instance, int instanceCount);

// set the partials for a given tip
//
// tipIndex the index of the tip
// inPartials the array of partials, stateCount x patternCount
int setPartials(
                    int instance,				/**< Instance number in which to set a partialsBuffer (input) */
             		int bufferIndex,			/**< Index of destination partialsBuffer (input) */
					const double* inPartials);	/**< Pointer to partials values to set (input) */

int getPartials(
		int instance,			/**< Instance number from which to get partialsBuffer (input) */
		int bufferIndex, 		/**< Index of source partialsBuffer (input) */
		double *outPartials		/**< Pointer to which to receive partialsBuffer (output) */
		);

// set the states for a given tip
//
// tipIndex the index of the tip
// inStates the array of states: 0 to stateCount - 1, missing = stateCount
int setTipStates(
                  int instance,			/**< Instance number (input) */
				  int tipIndex,			/**< Index of destination compressedBuffer (input) */
				  const int* inStates); /**< Pointer to compressed states (input) */



// sets the Eigen decomposition for a given matrix
//
// matrixIndex the matrix index to update
// eigenVectors an array containing the Eigen Vectors
// inverseEigenVectors an array containing the inverse Eigen Vectors
// eigenValues an array containing the Eigen Values
int setEigenDecomposition(
                           int instance,							/**< Instance number (input) */
						   int eigenIndex,							/**< Index of eigen-decomposition buffer (input) */
						   const double** inEigenVectors, 			/**< 2D matrix of eigen-vectors (input) */
						   const double** inInverseEigenVectors,	/**< 2D matrix of inverse-eigen-vectors (input) */
						   const double* inEigenValues); 			/**< 2D vector of eigenvalues*/

int setTransitionMatrix(	int instance,
                			int matrixIndex,
                			const double* inMatrix);


// calculate a transition probability matrices for a given list of node. This will
// calculate for all categories (and all matrices if more than one is being used).
//
// nodeIndices an array of node indices that require transition probability matrices
// edgeLengths an array of expected lengths in substitutions per site
// count the number of elements in the above arrays
int updateTransitionMatrices(
                                            int instance,	/**< Instance number (input) */
                                            int eigenIndex,	/**<  Index of eigen-decomposition buffer (input) */
                                            const int* probabilityIndices, /**<  List of indices of transition probability matrices to update (input) */
                                            const int* firstDerivativeIndices, /**< List of indices of first derivative matrices to update (input, NULL implies no calculation) */
                                            const int* secondDervativeIndices, /**< List of indices of second derivative matrices to update (input, NULL implies no calculation) */
                                            const double* edgeLengths, /**< List of edge lengths with which to perform calculations (input) */
                                            int count); /**< Length of lists */

// calculate or queue for calculation partials using an array of operations
//
// operations an array of triplets of indices: the two source partials and the destination
// dependencies an array of indices specify which operations are dependent on which (optional)
// count the number of operations
// rescale indicate if partials should be rescaled during peeling
/**
 * @brief Update or enqueue to update partialsBuffer
 *
 * LONG DESCRIPTION
 *
 * Operations list is a list of 5-tuple integer indices, with one 5-tuple per operation.
 * Format of 5-tuple operation: {destinationPartials,
 *                               child1Partials,
 *                               child1TransitionMatrix,
 *                               child2Partials,
 *                               child2TransitionMatrix}
 *
 */
int updatePartials(
                       int* instance, 		/**< List of instances for which to update partials buffers (input) */
                       int instanceCount, 	/**< Length of instance list (input) */
					   int* operations, 	/**< List of 5-tuples specifying operations (input) */
					   int operationCount, 	/**< Number of operations (input) */
					   int rescale); 		/**< Specify whether (=1) or not (=0) to recalculate scaling factors */

// calculate the site log likelihoods at a particular node
//
// rootNodeIndex the index of the root
// outLogLikelihoods an array into which the site log likelihoods will be put
int calculateRootLogLikelihoods(
                             int instance, /**< Instance number (input) */
		                     const int* bufferIndices, /**< List of partialsBuffer indices to integrate (input) */
		                     const double* weights, /**< List of weights to apply to each partialsBuffer (input) */
		                     const double** stateFrequencies, /**< List of state frequencies for each partialsBuffer (input)
															   * If list length is one, the same state frequencies are used
															   * for each partialsBuffer
															   */
		                     int count, /*< Number of partialsBuffer to integrate (input) */
			                 double* outLogLikelihoods); /**< Pointer to destination for resulting log likelihoods (output) */

// possible nulls: firstDerivativeIndices, secondDerivativeIndices,
//                 outFirstDerivatives, outSecondDerivatives
int calculateEdgeLogLikelihoods(
							 int instance, /**< Instance number (input) */
		                     const int* parentBufferIndices, /**< List of indices of parent partialsBuffers (input) */
		                     const int* childBufferIndices, /**< List of indices of child partialsBuffers (input) */
		                     const int* probabilityIndices , /**< List indices of transition probability matrices for this edge (input) */
		                     const int* firstDerivativeIndices, /**< List indices of first derivative matrices (input) */
		                     const int* secondDerivativeIndices, /**< List indices of second derivative matrices (input) */
		                     const double* weights, /**< List of weights to apply to each partialsBuffer (input) */
		                     const double** stateFrequencies, /**< List of state frequencies for each partialsBuffer (input)
															   * If list length is one, the same state frequencies are used
															   * for each partialsBuffer
															   */
		                     int count, /**< Number of partialsBuffers (input) */
		                     double* outLogLikelihoods, /**< Pointer to destination for resulting log likelihoods (output) */
			                 double* outFirstDerivatives, /**< Pointer to destination for resulting first derivatives (output) */
			                 double* outSecondDerivatives); /**< Pointer to destination for resulting second derivatives (output) */

#endif // __beagle__



