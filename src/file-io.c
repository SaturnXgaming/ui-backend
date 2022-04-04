#include <file-io.h>

plarray_t* plListDir(char* path, plgc_t* gc){
	DIR* dirStruct = opendir(path);

	if(!dirStruct){
		perror("plListDir");
		return NULL;
	}

	plarray_t* returnArray = plGCAlloc(gc, sizeof(plarray_t));
	returnArray->array = plGCAlloc(gc, 2 * sizeof(char*));
	returnArray->size = 0;
	struct dirent* workPtr;

	while((workPtr = readdir(dirStruct) != NULL){
		char filename[255] = "";

		memcpy(filename, workPtr->d_name);
		((char**)returnArray->array)[returnArray->size] = plGCAlloc(gc, strlen(filename));
		returnArray->size++;

		if(returnStruct->size > 1){
			void* tempPtr = plGCRealloc(gc, returnArray->array, (returnArray->size + 1) * sizeof(char*));

			if(!tempPtr){
				plShellFreeArray(returnArray, true, gc);
				return NULL;
			}

			returnArray->array = tempPtr;
		}
	}

	return returnArray;
}

size_t plGetFileSize(char* path){
	struct stat buf;

	if(stat(path, &buf)){
		perror("plGetFileSize");
		return 1;
	}

	return buf.st_size;
}

int plDeletePath(char* path, bool recursive){
	
}

int plOpenWith(char* path, char* command){
	
}
