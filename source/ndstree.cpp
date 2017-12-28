#include "ndstool.h"
#include "ndstree.h"
#include <vector>
#include <algorithm>


/*
 * Variables
 */
unsigned int _entry_start;			// current position in name entry table
unsigned int file_top;				// current position to write new file to
unsigned int free_dir_id = 0xF000;	// incremented in ReadDirectory
unsigned int directory_count = 0;	// incremented in ReadDirectory
unsigned int file_count = 0;		// incremented in ReadDirectory
unsigned int total_name_size = 0;	// incremented in ReadDirectory
unsigned int file_end = 0;			// end of all file data. updated in AddFile
unsigned int free_file_id = 0;		// incremented in AddDirectory

/*
 * ReadDirectory
 * Read directory tree into memory structure
 * returns first (dummy) node
 */
TreeNode *ReadDirectory(TreeNode *node, char *path)
{
	//printf("%s\n", path);

	DIR *dir = opendir(path);
	if (!dir) { fprintf(stderr, "Cannot open directory '%s'.\n", path); exit(1); }

	struct dirent *de;
	std::vector<dirent*> rep;
	while ((de = readdir(dir)))
		rep.push_back(de);

	std::sort(rep.begin(), rep.end(), [](dirent* a, dirent* b) {
		char* tempa = strdup(a->d_name);
		for(int i = 0; i < strlen(tempa); i++)
			tempa[i] = tolower(tempa[i]);
		char* tempb = strdup(b->d_name);
		for(int i = 0; i < strlen(tempb); i++)
			tempb[i] = tolower(tempb[i]);
		bool retour = strcmp(tempa, tempb) < 0;
		free(tempa); free(tempb);
		return retour;
	});

	for(dirent* de : rep)
	{
		if (!strncmp(de->d_name, ".", 1)) continue; // exclude all directories starting with .

		char strbuf[MAXPATHLEN];
		strcpy(strbuf, path);
		strcat(strbuf, "/");
		strcat(strbuf, de->d_name);
		//printf("%s\n", strbuf);

		struct stat st;
		if (stat(strbuf, &st)) { fprintf(stderr, "Cannot get stat of '%s'.\n", strbuf); exit(1); }

		//if (S_ISDIR(st.st_mode) && !subdirs) continue;		// skip subdirectories

		total_name_size += strlen(de->d_name);

		if (S_ISDIR(st.st_mode))
		{
			node = node->New(de->d_name, true);
			node->dir_id = free_dir_id++;
			directory_count++;
			node->directory = ReadDirectory(new TreeNode(), strbuf);
		}
		else if (S_ISREG(st.st_mode))
		{
			node = node->New(de->d_name, false);
			file_count++;
		}
		else
		{
			fprintf(stderr, "'%s' is not a file or directory!\n", strbuf);
			exit(1);
		}
	}
	closedir(dir);

	while (node->prev) node = node->prev;	// return first

	return node;
}
