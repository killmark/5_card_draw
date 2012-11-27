/* list data structure 
   under the hood each
   blob of data in the list
   is really an object of
   this struct type  */
typedef struct ListElmt_{
  void                *data;
  struct ListElmt_    *next;
} ListElmt;

typedef struct List_{
  int                 size;
  void                 (*destroy)(void *data); /* pointer to function telling
						 list how to destroy its members */
  int                 (*match)(const void *key1, const void *key2);
  ListElmt            *head;                  /* first element */
  ListElmt            *tail;                  /* last element (for convenience) */
} List;

/* initialize list instance*/
void list_init(List *list, void (*destroy)(void *data));
/* calls user-supplied destroy method on each element when finished with list */
void list_destroy(List *list);
/* create (malloc) a new ListElmt and place it after element in the list */
/* note that this assume the client has element. If so, this is a constant
   time operation. If not, it will need to be O(n) searched first.*/
int list_ins_next(List *list, ListElmt *element, const void *data);
/* remove ListElmt after element and assigns data pointer to ListElmt data */
int list_rem_next(List *list, ListElmt *element, const void **data);

/* accessor functions done as macros to minimize cost of frequent access
   on the one hand and to add a little abstraction on the other */
#define list_size(list) ((list)->size)
#define list_head(list) ((list)->head)
#define list_tail(list) ((list)->tail)
#define list_is_head(list,element) ((element) == (list)->head)
#define list_is_tail(list,element) ((element) == (list)->tail)
#define list_data(element) ((element)->data)
#define list_next(element) ((element)->next)

void list_print(List *list);
