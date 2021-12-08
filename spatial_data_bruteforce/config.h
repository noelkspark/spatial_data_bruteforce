#define data_size 10000000
#define MAX_HEAP_SIZE 100

#define BRUTE_FORCE 0
#define KD_TREE 1
#define R_TREE 2

#define RANGE 0
#define KNN 1

#define	CLUSTERED_INPUT_FILE			"geospatial_data/clustered_dataset.txt" 
#define	GAUSSIAN_INPUT_FILE				"geospatial_data/gaussian_dataset.txt" 
#define	UNIFORMED_INPUT_FILE			"geospatial_data/uniformed_dataset.txt"
#define	TEST_INPUT_FILE					"geospatial_data/test_dataset.txt" 


//////////*(1) CHOOSE MODE*/////////////////
////////// 1. BRUTE_FORCE //////////////////
////////// 2. KD_TREE //////////////////////
////////// 3. R_TREE ///////////////////////
#define MODE R_TREE
//////////*(2) CHOOSE QUERY*////////////////
////////// 1. RANGE ////////////////////////
////////// 2. KNN //////////////////////////
#define QUERY KNN

//////////*(3) CHOOSE INPUT DATASET FILE*///
////////// 1. CLUSTERED_INPUT_FILE /////////
////////// 2. GAUSSIAN_INPUT_FILE  /////////
////////// 3. UNIFORMED_INPUT_FILE /////////
////////// 4. TEST_INPUT_FILE ////////////// --> TEST YOUR OWN INPUT DATASET FILE
#define INPUT_FILE_NAME GAUSSIAN_INPUT_FILE
