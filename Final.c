#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    char* data;
    struct node* left;
    struct node* right;
} node;

node *newnode(char* value)
{
    node*n=malloc(sizeof(node));
    n->data=malloc(strlen(value)+1);
    strcpy(n->data,value);
    n->left=NULL;
    n->right=NULL;
    return n;
}

node* insert(node *root,char* value)
{
    if(root==NULL)
        root=newnode(value);
    else if(strcasecmp(root->data,value)<0)
        root->right=insert(root->right,value);
    else
        root->left=insert(root->left,value);
    return root;
}

node* load(node *root)
{
    FILE *f=fopen("Dictionary.txt","r");
    if (f == NULL)
    {
        printf("no such file.");
        return 0;
    }
    else
    {
        printf("Dictionary loaded successfully\n");
        printf("..................................\n");
    }
    char buffer[200];
    while(!feof(f))
    {
        fscanf(f,"%s",buffer);
        //printf("%s",buffer);
        root=insert(root,buffer);
    }
    fclose(f);
    return root;
}

int max(int a,int b)
{
    if(a>b)
        return a;
    else return b;
}

int height(node *root)
{
    if(root==NULL)
        return -1;
    else
    {
        return max(height(root->left),height(root->right))+1;
    }
}
int nodes(node *root)
{
    if(root==NULL)
        return 0;
    else
    {
        return nodes(root->left)+nodes(root->right)+1;
    }
}

inorder(node *root)
{
    if(root!=NULL)
    {
        inorder(root->left);
        printf("%s \n",root->data);
        inorder(root->right);
    }
}


node *search(node *root,char *value, node** lastNode)
{
    if(root==NULL)
        return NULL;
    *lastNode=root;
    if(strcasecmp(root->data,value)==0)
        return root;
    else if(strcasecmp(root->data,value)<0)
        return search(root->right,value,lastNode);
    else if(strcasecmp(root->data,value)>0)
        return search(root->left,value,lastNode);
}

node* getSuccessor(node* root, node* lastNode)
{
    if(lastNode==NULL)
        return NULL;
    if(lastNode->right!=NULL) //case 1 node has right subtree
    {
        node*temp= lastNode->right;
        while(temp->left!=NULL)
        {
            temp=temp->left;
        }
        return temp;
    }
    else //case 2 node has no right subtree
    {
        node* successor=NULL;
        node* ancestor= root;
        while(ancestor!=lastNode)
        {
            if(strcasecmp(lastNode->data,ancestor->data)<0) //check
            {
                successor=ancestor;
                ancestor=ancestor->left;
            }
            else
                ancestor=ancestor->right;
        }
        return successor;
    }
}

node* getPredecessor(node* root, node* lastNode)
{
    if(lastNode==NULL)
        return NULL;
     if(lastNode->left!=NULL) //case 1 node has left subtree
    {
        node*temp= lastNode->left;
        while(temp->right!=NULL)
        {
            temp=temp->right;
        }
        return temp;
    }
    else //case 2 node has no left subtree
    {
        node* predecessor=NULL;
        node* ancestor= root;
        while(ancestor!=lastNode)
        {
            if(strcasecmp(lastNode->data,ancestor->data)>0) //check
            {
                predecessor=ancestor;
                ancestor=ancestor->right;
            }
            else
                ancestor=ancestor->left;
        }
        return predecessor;
    }

}

void sentence(node* root)
{
    printf("Enter a sentence: ");
    char input[200];
    fgets(input,200,stdin);
    char delim[]=" \n";
    char* token=strtok(input,delim);
    node* lastNode=NULL;
    while(token)
    {
        node* found=search(root,token,&lastNode);
        if(found==NULL)
        {
            node* successor=getSuccessor(root, lastNode);
            node* predecessor=getPredecessor(root, lastNode);
            printf("%s - Incorrect, suggestions: %s %s %s\n",token,lastNode->data,successor->data,predecessor->data);
        }
        else
        {
            printf("%s - CORRECT\n",token);
        }
        token=strtok(NULL,delim);
    }
}

int main()
{
    node *root=NULL;
    root=load(root);
    //inorder(root);
    //node *found=search(root,"loop's");
    //printf("%s",found->data);
    printf("\nSize = %d\n",nodes(root));
    printf("..................................\n");
    printf("\nHeight = %d\n",height(root));
    printf("..................................\n");
    sentence(root);
    return 0;
}
