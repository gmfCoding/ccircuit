#include <stddef.h>

typedef struct s_array t_array;
typedef struct s_opt t_opt;
typedef int (*t_cmpfunc)(t_array *, void*, void*);
typedef unsigned short t_asize;

struct s_array
{
	t_asize count;
	t_asize capacity;
	t_asize ofsize;

	void *elem;
};

void	*array_findf(t_array *arr, void *find, t_cmpfunc func);
void	*array_find(t_array *array, void *find);
t_array	array_init(t_asize capacity, t_asize ofsize);
void	*array_get(t_array *arr, t_asize i);
void	array_add(t_array *arr, void *item);
