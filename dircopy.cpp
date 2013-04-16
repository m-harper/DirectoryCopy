#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <iostream>
#include <cstdlib>
#include <string>
#include <fstream>

#define true 1
#define false 0
#define bool int

using namespace std;

bool copy_folder(const char*, const char*);
bool copy_file(string, string);
void parse_args(int, char**);

string src;
string dest;

int main(int argc, char** argv) {
	parse_args(argc, argv);

	mkdir(dest.c_str(), 0777);
	copy_folder(src.c_str(), dest.c_str());

	return 0;
}

bool copy_folder(const char* src, const char* dest) {
	DIR* root = opendir(src);
	struct dirent* child;

	if (root == NULL) {
		cerr << "Error opening " << src << endl;
		return false;
	}

	while (child = readdir(root)) {
		string name = string(child->d_name);
		if (name == "." || name == "..") {
			continue;
		}

		if (child->d_type == DT_DIR) {
			mkdir((string(dest) + name).c_str(), 0777);
			copy_folder((string(src) + name).c_str(), (string(dest) + name).c_str());
		} else {
			if (! copy_file(string(src) + name, string(dest) + "/" + name)) {
				cerr << "Error, failed to copy " << name << endl;
			}
		}
	}

	closedir(root);
}

bool copy_file(string src, string dest) {
	ifstream infile;
	ofstream outfile;
	infile.open(src.c_str(), ios_base::binary);
	outfile.open(dest.c_str(), ios_base::binary);

	int ch = infile.get();
	while (infile.good()) {
		outfile << (char) ch;
		ch = infile.get();
	}

	infile.close();
	outfile.close();
}

void parse_args(int argc, char** argv) {
	if (argc != 3) {
		cerr << "Correct usage is " << argv[0] << " src dest" << endl;
		exit(1);
	}

	src = string(argv[1]);
	if (src[src.length() - 1] != '/') {
		src += '/';
	}
	dest = string(argv[2]);	
	if (dest[dest.length() - 1] != '/') {
		dest += '/';
	}
}
