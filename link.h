struct node 	//structure for node
{	
   long data;
   struct node *next;
};

struct node *head = NULL;
struct node *current = NULL;


//insert data at the first location
void insertFirst( long data) 
{
   //create a node 
   struct node *link = (struct node*) malloc(sizeof(struct node));
	
   link->data = data;
	
   //point it to old first node
   link->next = head;
	
   //point first to new first node
   head = link;
}


//find a node with given data
int find(long data) 
{

   //start from the first node
   struct node* current = head;

   //if list is empty
   if(head == NULL) 
   {
      return -1;
   }

   //navigate through list
   while(current->data != data) 
   {
	
      //if it is last node
      if(current->next == NULL) 
      {        
	return -1;
      } 
      else 
      {
         //go to next node
         current = current->next;
      }
   }      
	
   //if data found, return 1
   return 1;
}


