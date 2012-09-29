#include<stdio.h>
#include<stdlib.h>
#include<cstring>
#include"texteditor.h"


/* This function create the memory for a node from the free list */
text_t *get_node()
{ 
	text_t *tmp;
  	nodes_taken += 1;
  	if (free_list != NULL)
  	{  
		tmp = free_list;
     		free_list = free_list -> right;
  	}
  	else
  	{  
		if (currentblock == NULL || size_left == 0)
     		{	  
			currentblock = (text_t *) malloc(BLOCKSIZE * sizeof(text_t));
        		size_left = BLOCKSIZE;
     		}
     		tmp = currentblock++;
     		size_left -= 1;
  	}
  	return (tmp);
}

/* This function returns the deleted node to the free list */
void return_node(text_t *node)
{  
	node->right = free_list;
   	free_list = node;
   	nodes_returned +=1;
}

/* This function assigns the default values for each node */
text_t *create_node(void)
{  
	text_t *tmp_node;
   	tmp_node = get_node();
   	tmp_node->left = NULL;
	tmp_node->right = NULL;
	tmp_node->parent = NULL;
   	tmp_node->key = -1;
	tmp_node->height = -1;
	return (tmp_node);
}

/* This function assigns the given line to a leaf node */
object_t *create_object(char *new_line)
{
	object_t *tmp_obj = (object_t *) malloc(strlen(new_line)+1);
	if (tmp_obj == NULL)
	{
		perror("Malloc failed in create object");
		return NULL;
	}
	strcpy(tmp_obj, new_line);
	return (tmp_obj);
}


/* This function is used to find the height of a tree or sub-tree */
int max_height(text_t *txt) 
{
	int ldepth, rdepth;
	if (txt->right == NULL) 
	{
		return 0;
	}
   	else
   	{
   		ldepth = max_height(txt->left);
       		rdepth = max_height(txt->right);
       		if (ldepth < rdepth) 
		{
           		return (rdepth+1);
       		}
		else
		{ 
			return (ldepth+1);
   		}
	}
} 

/* This function initialises the balanced tree data structure */
text_t *create_text()
{	
	text_t *tmp_node;
	tmp_node = create_node();
	return (tmp_node);
}

/* This function returns the number of lines of the current text */
int length_text(text_t *txt)
{
	if (txt->left == NULL || txt == NULL)
	{
		return 0; 
	}
	else
	{
		return (txt->key);
	}
} 

/* This function performs the left rotation of a tree */
void left_rotation(text_t *txt)
{ 
	text_t *tmp_node;
	tmp_node = txt->left;
	txt->left = txt->right;
	txt->right = txt->left->right;
	txt->right->parent = txt; 
	txt->left->right = txt->left->left;
	txt->left->left = tmp_node;
        tmp_node->parent = txt->left; 
}

/* This function performs the right rotation of a tree */
void right_rotation(text_t *txt)
{ 
	text_t *tmp_node;
	tmp_node = txt->right;
	txt->right = txt->left;
	txt->left = txt->right->left;
	txt->left->parent = txt; 
	txt->right->left = txt->right->right;
	txt->right->right = tmp_node;
        tmp_node->parent = txt->right; 
}

/* This function inserts the line before the line of number index, if such a line exists, to new line, renumbering all lines after that line. If no such line exists, it appends new line as new last line */
void insert_line(text_t *txt, int index, char * new_line) 
{
	text_t *tmp_node = txt;
	int finished;
	int count = index;
	if (txt->left == NULL)
	{
		txt->left = (text_t *) create_object(new_line); 
      		txt->key  = 1;
      		txt->right  = NULL;
		txt->parent = NULL;
		txt->height = 0;
	}
	else if (index > (tmp_node->key))
	{
		append_line(txt, new_line);
	}	
	else
	{
		while(tmp_node->right != NULL)
        	{
        		(tmp_node->key)++;
			if (count <= tmp_node->left->key)
			{
				tmp_node = tmp_node->left;
			}
			else
			{
				count -= tmp_node->left->key;
				tmp_node = tmp_node->right;
			}
		}

	        text_t *old_leaf, *new_leaf;
        	old_leaf = get_node();
        	new_leaf = get_node();
        	old_leaf->left = tmp_node->left;
        	old_leaf->key = 1;
        	old_leaf->right  = NULL;
        	old_leaf->parent = tmp_node;
        	old_leaf->height = 0;

        	new_leaf->left = (text_t *) create_object(new_line);
        	new_leaf->key = 1;
        	new_leaf->right  = NULL;
        	new_leaf->parent = tmp_node;
        	new_leaf->height = 0;
        	tmp_node->right = old_leaf;
        	tmp_node->left = new_leaf;
		(tmp_node->key)++; 
		tmp_node->height = 0;
		  
		finished = 0;
		while(!finished && tmp_node!=NULL)
		{
			int tmp_height, old_height;
			old_height = tmp_node->height;
			if (tmp_node->left->height - tmp_node->right->height == 2)
			{ 
				if (tmp_node->left->left->height - tmp_node->right->height == 1)
				{ 
					right_rotation(tmp_node);
					tmp_node->right->height = tmp_node->right->left->height + 1;
					tmp_node->height = tmp_node->right->height + 1;
					tmp_node->right->key = tmp_node->right->left->key + tmp_node->right->right->key;
					tmp_node->key = tmp_node->left->key + tmp_node->right->key;
				}
				else
				{ 
					left_rotation(tmp_node->left);
					right_rotation(tmp_node);
					tmp_height = tmp_node->left->left->height;
					tmp_node->left->height = tmp_height + 1;
					tmp_node->right->height = tmp_height + 1;
					tmp_node->height = tmp_height + 2;
					tmp_node->left->key = tmp_node->left->left->key + tmp_node->left->right->key;
					tmp_node->right->key = tmp_node->right->left->key + tmp_node->right->right->key;
                                        tmp_node->key = tmp_node->left->key + tmp_node->right->key;
				}
			}
			else if (tmp_node->left->height - tmp_node->right->height == -2)
			{ 
				if( tmp_node->right->right->height - tmp_node->left->height == 1)
				{ 
					left_rotation(tmp_node);
					tmp_node->left->height = tmp_node->left->right->height + 1;
					tmp_node->height = tmp_node->left->height + 1;
					tmp_node->left->key = tmp_node->left->left->key + tmp_node->left->right->key;
                                        tmp_node->key = tmp_node->left->key + tmp_node->right->key;
				}
				else
				{ 
					right_rotation(tmp_node->right);
					left_rotation(tmp_node);
					tmp_height = tmp_node->right->right->height;
					tmp_node->left->height = tmp_height + 1;
					tmp_node->right->height = tmp_height + 1;
					tmp_node->height = tmp_height + 2;
					tmp_node->right->key = tmp_node->right->left->key + tmp_node->right->right->key;
					tmp_node->left->key = tmp_node->left->left->key + tmp_node->left->right->key;
                                        tmp_node->key = tmp_node->left->key + tmp_node->right->key;
				}
			}
      			else 
			{ 
				if(tmp_node->left->height > tmp_node->right->height)
				{
					tmp_node->height = tmp_node->left->height + 1;
				}
				else
				{
					tmp_node->height = tmp_node->right->height + 1;
				}
			}
			if (tmp_node->height == old_height)
			{
				finished = 1;
			}
			tmp_node = tmp_node->parent;
		}
	}

}

/* This function appends new line as new last line */
void append_line(text_t *txt, char *new_line) 
{
	text_t *tmp_node = txt;
	int finished;
	if (txt->left == NULL)
	{
		txt->left = (text_t *) create_object(new_line);
		txt->right = NULL;
		txt->parent = NULL;
		txt->key = 1;
		txt->height = 0;
	}
	else
	{
		while(tmp_node->right != NULL)
		{
			(tmp_node->key)++;
			tmp_node = tmp_node->right;
		}

		text_t *old_leaf, *new_leaf;
		old_leaf = get_node();
		new_leaf = get_node();
		old_leaf->left = tmp_node->left;
         	old_leaf->key = tmp_node->key;
         	old_leaf->right  = NULL;
		old_leaf->parent = tmp_node;	
		old_leaf->height = 0;

		new_leaf->left = (text_t *) create_object(new_line);
         	new_leaf->key = tmp_node->key;
         	new_leaf->right  = NULL;
		new_leaf->parent = tmp_node;
		new_leaf->height = 0;
		tmp_node->left = old_leaf;
		tmp_node->right = new_leaf;	
		(tmp_node->key)++;		
		tmp_node->height = 0;

		finished = 0;
		while(!finished && tmp_node!=NULL)
		{
			int tmp_height, old_height;
			old_height = tmp_node->height;
			if (tmp_node->left->height - tmp_node->right->height == 2)
			{ 
				if (tmp_node->left->left->height - tmp_node->right->height == 1)
				{ 
					right_rotation(tmp_node);
					tmp_node->right->height = tmp_node->right->left->height + 1;
					tmp_node->height = tmp_node->right->height + 1;
					tmp_node->right->key = tmp_node->right->left->key + tmp_node->right->right->key;
					tmp_node->key = tmp_node->left->key + tmp_node->right->key;
				}
				else
				{ 
					left_rotation(tmp_node->left);
					right_rotation(tmp_node);
					tmp_height = tmp_node->left->left->height;
					tmp_node->left->height = tmp_height + 1;
					tmp_node->right->height = tmp_height + 1;
					tmp_node->height = tmp_height + 2;
					tmp_node->left->key = tmp_node->left->left->key + tmp_node->left->right->key;
					tmp_node->right->key = tmp_node->right->left->key + tmp_node->right->right->key;
                                        tmp_node->key = tmp_node->left->key + tmp_node->right->key;
				}
			}
			else if (tmp_node->left->height - tmp_node->right->height == -2)
			{ 
				if( tmp_node->right->right->height - tmp_node->left->height == 1)
				{ 
					left_rotation(tmp_node);
					tmp_node->left->height = tmp_node->left->right->height + 1;
					tmp_node->height = tmp_node->left->height + 1;
					tmp_node->left->key = tmp_node->left->left->key + tmp_node->left->right->key;
                                        tmp_node->key = tmp_node->left->key + tmp_node->right->key;
				}
				else
				{ 
					right_rotation(tmp_node->right);
					left_rotation(tmp_node);
					tmp_height = tmp_node->right->right->height;
					tmp_node->left->height = tmp_height + 1;
					tmp_node->right->height = tmp_height + 1;
					tmp_node->height = tmp_height + 2;
					tmp_node->right->key = tmp_node->right->left->key + tmp_node->right->right->key;
					tmp_node->left->key = tmp_node->left->left->key + tmp_node->left->right->key;
                                        tmp_node->key = tmp_node->left->key + tmp_node->right->key;
				}
			}
      			else 
			{ 
				if(tmp_node->left->height > tmp_node->right->height)
				{
					tmp_node->height = tmp_node->left->height + 1;
				}
				else
				{
					tmp_node->height = tmp_node->right->height + 1;
				}
			}
			if (tmp_node->height == old_height)
			{
				finished = 1;
			}
			tmp_node = tmp_node->parent;
		}
	}
}

/* This function gets the line of number index, if such a line exists, and returns NULL else */
char *get_line(text_t *txt, int index) 
{
	int count = index;
	text_t *tmp_node = txt;
	if (txt->key < index || index == 0 || txt->key == -1)
	{
		return NULL;
	}
	else
	{
		while(tmp_node->right != NULL)
                {
                        if (count <= tmp_node->left->key)
                        {
                                tmp_node = tmp_node->left;
                        }
                        else
                        {
                                count -= tmp_node->left->key;
                                tmp_node = tmp_node->right;
                        }
                }
		return (object_t *)(tmp_node->left);
	}
} 

/* This function sets the line of number index, if such a line exists, to new line, and returns a pointer to the previous line of that number. If no line of that number exists, it does not change the structure and returns NULL */
char *set_line( text_t *txt, int index, char * new_line)
{
	int count = index;
	char *old_line;
        text_t *tmp_node = txt;
        if (txt->key < index || index == 0 || txt->key == -1)
        {
                return NULL;
        }
        else
        {
                while(tmp_node->right != NULL)
                {
                        if (count <= tmp_node->left->key)
                        {
                                tmp_node = tmp_node->left;
                        }
                        else
                        {
                                count -= tmp_node->left->key;
                                tmp_node = tmp_node->right;
                        }
                }

		old_line = (object_t *) create_object(((object_t *)(tmp_node->left)));
		tmp_node->left = (text_t *) create_object(new_line);
                return old_line;
        }
} 

/* This function deletes the line of number index, renumbering all lines after that line, and returns a pointer to the deleted line */
char * delete_line(text_t *txt, int index) 
{
	text_t *tmp_node, *upper_node, *other_node;
   	object_t *deleted_object;
	int count = index;
	int finished;
   	tmp_node = txt;
	if (txt->left == NULL || length_text(txt) < index)
	{
		return NULL;
	}
	else if (txt->right == NULL)
	{
		deleted_object = (object_t *)txt->left;
		txt->left = NULL;
		txt->parent = NULL;
		txt->key = -1;
		txt->height = -1;
		return (deleted_object);
	}
	else
	{
		while(tmp_node->right != NULL)
      		{   
			upper_node = tmp_node;
			(tmp_node->key)--;
                        if (count <= tmp_node->left->key)
                        {
                                tmp_node = tmp_node->left;
				tmp_node = upper_node->left;
                        	other_node = upper_node->right;
                        }
                        else
                        {
                                count -= tmp_node->left->key;
                                tmp_node = tmp_node->right;
				tmp_node = upper_node->right;
				other_node = upper_node->left;
                        }
      		}
		upper_node->key   = other_node->key;
         	upper_node->left  = other_node->left;
         	upper_node->right = other_node->right;
                upper_node->height = other_node->height;
		if (upper_node->right != NULL)
		{
			upper_node->left->parent = upper_node;
			upper_node->right->parent = upper_node;
         	}
		deleted_object = (object_t *) tmp_node->left;
         	return_node(tmp_node);
         	return_node(other_node);
		upper_node = upper_node->parent;
		tmp_node = upper_node;
 
		finished = 0;
		while(!finished && tmp_node!=NULL)
		{
			int tmp_height, old_height;
			old_height = tmp_node->height;
			if (tmp_node->left->height - tmp_node->right->height == 2)
			{ 
				if (tmp_node->left->left->height - tmp_node->right->height == 1)
				{ 
					right_rotation(tmp_node);
					tmp_node->right->height = tmp_node->right->left->height + 1;
					tmp_node->height = tmp_node->right->height + 1;
					tmp_node->right->key = tmp_node->right->left->key + tmp_node->right->right->key;
					tmp_node->key = tmp_node->left->key + tmp_node->right->key;
				}
				else
				{ 
					left_rotation(tmp_node->left);
					right_rotation(tmp_node);
					tmp_height = tmp_node->left->left->height;
					tmp_node->left->height = tmp_height + 1;
					tmp_node->right->height = tmp_height + 1;
					tmp_node->height = tmp_height + 2;
					tmp_node->left->key = tmp_node->left->left->key + tmp_node->left->right->key;
					tmp_node->right->key = tmp_node->right->left->key + tmp_node->right->right->key;
                                        tmp_node->key = tmp_node->left->key + tmp_node->right->key;
				}
			}
			else if (tmp_node->left->height - tmp_node->right->height == -2)
			{ 
				if( tmp_node->right->right->height - tmp_node->left->height == 1)
				{ 
					left_rotation(tmp_node);
					tmp_node->left->height = tmp_node->left->right->height + 1;
					tmp_node->height = tmp_node->left->height + 1;
					tmp_node->left->key = tmp_node->left->left->key + tmp_node->left->right->key;
                                        tmp_node->key = tmp_node->left->key + tmp_node->right->key;
				}
				else
				{ 
					right_rotation(tmp_node->right);
					left_rotation(tmp_node);
					tmp_height = tmp_node->right->right->height;
					tmp_node->left->height = tmp_height + 1;
					tmp_node->right->height = tmp_height + 1;
					tmp_node->height = tmp_height + 2;
					tmp_node->right->key = tmp_node->right->left->key + tmp_node->right->right->key;
					tmp_node->left->key = tmp_node->left->left->key + tmp_node->left->right->key;
                                        tmp_node->key = tmp_node->left->key + tmp_node->right->key;
				}
			}
      			else 
			{ 
				if(tmp_node->left->height > tmp_node->right->height)
				{
					tmp_node->height = tmp_node->left->height + 1;
				}
				else
				{
					tmp_node->height = tmp_node->right->height + 1;
				}
			}
			if (tmp_node->height == old_height)
			{
				finished = 1;
			}
			tmp_node = tmp_node->parent;
		}

	       	return (deleted_object);
	}
}

/* This function performs the inorder traversal of a given tree */
void inorder(text_t *txt)
{
	if (txt->right == NULL)
	{
		printf("************************\n");
                printf("The key is %d\n",txt->key);
                printf("The height is %d\n",txt->height);
                printf("The text is %s\n", (object_t *)txt->left);
                printf("The parent address is %d\n", txt->parent);
                
		printf("************************\n");
		return;
	}
	else if (txt != NULL)
	{
		inorder(txt->left);
		printf("************************\n");
		printf("The key is %d\n",txt->key);
		printf("The height is %d\n",txt->height);
		printf("The text is %s\n", (object_t *)txt->left);
		printf("The parent address is %d\n", txt->parent);
		printf("************************\n");	
		inorder(txt->right);
	}
}

/* This function prints the all the elements of the given node */
void display(text_t *txt)
{
        if (txt != NULL)
        {
                printf("\n\n ************************\n");
                printf("The key is %d\n",txt->key);
                printf("The height is %d\n",txt->height);
                printf("The text is %s", (object_t *)txt->left);
                printf("************************ \n\n");
        }
	else
	{
		printf("Node is NULL\n");
	}
}


/* Test cases for the texteditor */
int main()
{  

	int i, tmp; text_t *txt1, *txt2; char *c;
   	printf("starting \n");
   	txt1 = create_text();
   	txt2 = create_text();
   	append_line(txt1, "line one" );
   	if( (tmp = length_text(txt1)) != 1)
   	{  
		printf("Test 1: length should be 1, is %d\n", tmp); exit(-1);
   	}
   	append_line(txt1, "line hundred" );
   	insert_line(txt1, 2, "line ninetynine" );
   	insert_line(txt1, 2, "line ninetyeight" );
   	insert_line(txt1, 2, "line ninetyseven" );
   	insert_line(txt1, 2, "line ninetysix" );
   	insert_line(txt1, 2, "line ninetyfive" );
   	for( i = 2; i <95; i++ )
     		insert_line(txt1, 2, "some filler line between 1 and 95" );
   	if( (tmp = length_text(txt1)) != 100)
   	{  
		printf("Test 2: length should be 100, is %d\n", tmp); exit(-1);
   	}
   	printf("found at line 1:   %s\n",get_line(txt1,  1));
   	printf("found at line 2:   %s\n",get_line(txt1,  2));
   	printf("found at line 99:  %s\n",get_line(txt1, 99));
   	printf("found at line 100: %s\n",get_line(txt1,100));
   	for(i=1; i<=10000; i++)
   	{  
		if( i%2==1 )
        		append_line(txt2, "A");
      		else 
        		append_line(txt2, "B");
   	}
   	if( (tmp = length_text(txt2)) != 10000)
   	{  
		printf("Test 3: length should be 10000, is %d\n", tmp); exit(-1);
   	}
   	c = get_line(txt2, 9876 );
   	if( *c != 'B')
     	{  
		printf("Test 4: line 9876 of txt2 should be B, found %s\n", c); exit(-1);
   	}
   	for( i= 10000; i > 1; i-=2 )
   	{  
		c = delete_line(txt2, i);
      		if( *c != 'B')
      		{  
			printf("Test 5: line %d of txt2 should be B, found %s\n", i, c); exit(-1);
      		}
      		append_line( txt2, c );
   	}
   	for( i=1; i<= 5000; i++ )
   	{  
		c = get_line(txt2, i);
      		if( *c != 'A')
      		{  
			printf("Test 6: line %d of txt2 should be A, found %s\n", i, c); exit(-1);
      		}
   	}
   	for( i=1; i<= 5000; i++ )
     		delete_line(txt2, 1 );
   	for( i=1; i<= 5000; i++ )
   	{  
		c = get_line(txt2, i);
      		if( *c != 'B')
      		{  
			printf("Test 7: line %d of txt2 should be B, found %s\n", i, c); exit(-1);
      		}
   	}
   	set_line(txt1, 100, "the last line");
   	for( i=99; i>=1; i-- )
     		delete_line(txt1, i );
   	printf("found at the last line:   %s\n",get_line(txt1,  1));
	//inorder(txt2);   
}
