#include <filesystem>
#include <cstdio>

int main() {
	const std::filesystem::path& path = "folder\\";

	FILE* pFile, *pFile1;

	char key = '¤';

	//get the folders 'content'
	for (const std::filesystem::directory_entry &p : std::filesystem::directory_iterator(path)) {
		std::filesystem::path filename = p.path().filename();
		std::string tmp = path.string() + filename.string();

		fopen_s(&pFile, tmp.c_str(), "rb");
		
		if (pFile) {
			//for size purposes
			char* buffer = 0;
			long size;
			fseek(pFile, 0L, SEEK_END);
			size = ftell(pFile);
			fseek(pFile, 0L, SEEK_SET); //set position to beginning of file rewind(pFile) works too

			buffer = new char[size+1];

			//set to zero to remove random chars
			memset(buffer, 0, size+1);

			//get filecontent and feed buffer
			fread(buffer, sizeof(char), size, pFile);
			
			fclose(pFile);

			//XOR-crypt text/data
			for (int i = 0; i < size+1; i++) {
				buffer[i] ^= key;
			}

			//open for writing
			fopen_s(&pFile1, tmp.c_str(), "wb");

			//write back to file
			fwrite(buffer, sizeof(char), size, pFile1);

			fclose(pFile1);
			delete[] buffer;
		}
	}
}
