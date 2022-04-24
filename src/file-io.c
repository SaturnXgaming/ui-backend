#include <file-io.h>

plarray_t* plListDir(char* path){
	DIR* dirStruct = opendir(path);

	if(!rootGC)
		plGCInit(rootGC, 0);

	if(!dirStruct){
		perror("plListDir");
		return NULL;
	}

	plarray_t* returnArray = plGCAlloc(rootGC, sizeof(plarray_t));
	returnArray->array = plGCAlloc(rootGC, 2 * sizeof(char*));
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

struct stat* plGetFileInfo(char* path){
	if(!rootGC)
		plGCInit(rootGC, 0);

	struct stat* buf = plGCAlloc(rootGC, sizeof(struct stat));

	if(stat(path, buf)){
		perror("plGetFileInfo");
		return NULL;
	}

	return buf;
}

int plGetFileSize(char* path){
	struct stat* buf = plGetFileInfo(path)
	if(!buf)
		return -1;

	size_t retSize = buf->st_size;
	plGCFree(rootGC, buf);

	return retSize;
}

int plDeletePath(char* path, bool recursive){
	struct stat* buf = plGetFileInfo(path);
	if(!buf)
		return 1;
	plGCFree(rootGC, buf);

	if(recursive){
		plarray_t* dirList = plListDir(path);
		if(!dirList)
			return 2;

		const char** array = dirList->array;
		for(int i = 0; i < dirList->size; i++){
			struct stat* buf = plGetFileInfo(array[i]);
			if(!buf)
				return 3;

			if(S_ISREG(buf->st_mode)){
				plGCFree(rootGC, buf);
				if(plDeletePath(array[i], true) != 0)
					return 4;

				if(rmdir(array[i]) == -1){
					perror("plDeletePath");
					return 5;
				}
			}else{
				plGCFree(rootGC, buf);
				if(remove(array[i]) == -1){
					perror("plDeletePath");
					return 6;
				}
			}
		}
	}

	return 0;
}

int plOpenWith(char* path, char* command){
	if(!rootGC)
		plGCInit(rootGC, 0);

	if(fork() == 0)
		exec(command, path);

	return 0;
}
