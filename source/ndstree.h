#include <cctype>


inline int cmp(char *a, bool a_isdir, char *b, bool b_isdir)
{
	// oh... directory sort doesn't matter since we write out dir- and filenames seperately
	//if (a_isdir && !b_isdir) return -1;
	//if (b_isdir && !a_isdir) return +1;
	char* tempa = new char[strlen(a)+1];
	strcpy(tempa, a);
	for(int i = 0; i < strlen(tempa); i++)
		tempa[i] = tolower(tempa[i]);
	char* tempb = new char[strlen(b)+1];
	strcpy(tempb, b);
	for(int i = 0; i < strlen(tempb); i++)
		tempb[i] = tolower(tempb[i]);
	int retour = strcmp(tempa, tempb);
	delete[] tempa;
	delete[] tempb;
	return retour;
}

struct TreeNode
{
	unsigned int dir_id;		// directory ID in case of directory entry
	char *name;					// file or directory name
	TreeNode *directory;		// nonzero indicates directory. first directory node is a dummy
	TreeNode *prev, *next;		// linked list

	TreeNode()
	{
		dir_id = 0;
		name = (char *)"";
		directory = 0;
		prev = next = 0;
	}
	
	// new entry in same directory
	TreeNode *New(char *name, bool isdir)
	{
		TreeNode *newNode = new TreeNode();
		newNode->name = strdup(name);

		TreeNode *node = this;

		if (cmp(name, isdir, node->name, node->dir_id) < 0)	// prev
		{
			while (cmp(name, isdir, node->name, node->dir_id) < 0)
			{
				if (node->prev)
					node = node->prev;
				else
					break;		// insert after dummy node
			}
		}
		else
		{
			while (node->next && (cmp(name, isdir, node->next->name, node->next->dir_id) >= 0))
			{
				node = node->next;
			}
		}

		// insert after current node
		newNode->prev = node;
		newNode->next = node->next;
		if (node->next) node->next->prev = newNode;
		node->next = newNode;

		return newNode;
	}
};

TreeNode *ReadDirectory(TreeNode *node, char *path);
