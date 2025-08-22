/*
Contents:
* VP_ReadFile() --- Which Reads files, requires STDIO, STDLIB, fcntl, stat, & unistd
* VP_VFCompile() --- Which build shaders from read files, requires All of the above + An OpenGL Context
* VP_LoadVox() --- Which builds a float array (for voxel positions) from a read file.
*/

// Header Guard <3
#ifndef VP_BUILDSHADER_H
#define VP_BUILDSHADER_H

char* VP_ReadFile(char *Path, int *OpSize) {
	int OFDesc; // Open File Description
	if ( !(OFDesc = open(Path, 0, 00400)) ) { // FLAGS: O_RDONLY and S_IRUSR
		printf("FILE COULD NOT BE OPENED\n");
	}
	else {
		// FILE STAT INFO SEGMENT
		struct stat Finfo;
		fstat(OFDesc, &Finfo);

		// FILE READING SEGMENT
		char *Fstore = malloc(Finfo.st_size);
		if ( read(OFDesc, Fstore, Finfo.st_size) >= 0 ) {
			close(OFDesc);

			if (OpSize != NULL) {
				*OpSize = Finfo.st_size;
			}

			return (Fstore);
		}
		else {
			printf("FILE COULD NOT BE READ\n");
			close(OFDesc);
			free(Fstore); Fstore = NULL;
			return (Fstore);
		}
	}
}

// Note: May be moved to a seperate Header
float* VP_LoadVox(char* FilePath, int *OpSize) {
	// WARNING: Values read may experience floating point precission errors if they're smaller than one decimal/radix point.

	// ReadFile() SECTION
	int ByteSize;
	char *RawVoxOut = VP_ReadFile(FilePath, &ByteSize);
	float* Output = malloc( (sizeof(float)*(ByteSize/8)) ); // 8 is a magic number based on the amount of chars in the file format.

	// FLOAT CONVERSION SECTION
	unsigned int VoxelIndex = 0;
	for (unsigned int i = 0; i <= ByteSize; i++) {
		if (RawVoxOut[i] == ',') {
			char buffer[7] = {RawVoxOut[i-6], RawVoxOut[i-5], RawVoxOut[i-4], RawVoxOut[i-3], RawVoxOut[i-2], RawVoxOut[i-1]};
			Output[VoxelIndex] = strtof(buffer, NULL);
			VoxelIndex++;
		}
	}
	free(RawVoxOut);

	if (OpSize != NULL) {
		*OpSize = ByteSize;
	}
	return Output;
}

void VP_CompileShaders(unsigned int *ShaderProgramID, char *VFilePath, char *GFilePath, char *FFilePath){
	char *VShader;
	unsigned int VertShaderID;

	char *GShader;
	unsigned int GeoShaderID;

	char *FShader;
	unsigned int FragShaderID;

	if (VFilePath != NULL){
		// Vertex Shader Compilation
		VShader = VP_ReadFile(VFilePath, NULL);
		VertShaderID = glCreateShader(GL_VERTEX_SHADER); // WIP: ADD ERROR CHECK (0 == ERROR)
		glShaderSource(VertShaderID, 1, (const char * const *)&VShader, NULL); // Cast forces char* to const char*: (const char * const *)
		glCompileShader(VertShaderID);
			// Shader Compilation Check
		int Vcheck; char VInfoLog[512];
		glGetShaderiv(VertShaderID, GL_COMPILE_STATUS, &Vcheck);
		if (!Vcheck){
			glGetShaderInfoLog(VertShaderID, 512, NULL, VInfoLog);
			printf("VSHADER COMPILATION FAIL:\n%s\n", VInfoLog);
		}
	}
	else {
		VShader = NULL;
	}

	if (GFilePath != NULL){
		GShader = VP_ReadFile(GFilePath, NULL);
		// Geometry Shader Compilation
		GeoShaderID = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(GeoShaderID, 1, (const char * const *)&GShader, NULL); // Cast forces char* to const char*: (const char * const *)
		glCompileShader(GeoShaderID);
			// Shader Compilation Check
		int Gcheck; char GInfoLog[512];
		glGetShaderiv(GeoShaderID, GL_COMPILE_STATUS, &Gcheck);
		if (!Gcheck){
			glGetShaderInfoLog(GeoShaderID, 512, NULL, GInfoLog);
			printf("GSHADER COMPILATION FAIL:\n%s\n", GInfoLog);
		}
	}
	else {
		GShader = NULL;
	}

	if (FFilePath != NULL){
		FShader = VP_ReadFile(FFilePath, NULL);
		// Fragment Shader Compilation
		FragShaderID = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(FragShaderID, 1, (const char * const *)&FShader, NULL); // Cast forces char* to const char*: (const char * const *)
		glCompileShader(FragShaderID);
			// Shader Compilation Check
		int Fcheck; char FInfoLog[512];
		glGetShaderiv(FragShaderID, GL_COMPILE_STATUS, &Fcheck);
		if (!Fcheck){
			glGetShaderInfoLog(FragShaderID, 512, NULL, FInfoLog);
			printf("FSHADER COMPILATION FAIL:\n%s\n", FInfoLog);
		}
	}
	else {
		FShader = NULL;
	}

	// Final Shader Program Linkage
	*ShaderProgramID = glCreateProgram();
	glAttachShader(*ShaderProgramID, VertShaderID);
	glAttachShader(*ShaderProgramID, FragShaderID);
	glAttachShader(*ShaderProgramID, GeoShaderID);
	glLinkProgram(*ShaderProgramID);
		// Shader Linkage Check
	int SPcheck; char SPInfoLog[512];
	glGetProgramiv(*ShaderProgramID, GL_LINK_STATUS, &SPcheck);
	if (!SPcheck){
		glGetProgramInfoLog(*ShaderProgramID, 512, NULL, SPInfoLog);
		printf("SHADER LINKAGE FAIL:\n%s\n", SPInfoLog);
	}

	// Cleanup
	glDeleteShader(VertShaderID);
	glDeleteShader(GeoShaderID);
	glDeleteShader(FragShaderID);
	free(VShader); VShader = NULL;
	free(GShader); FShader = NULL;
	free(FShader); GShader = NULL;
}

#endif