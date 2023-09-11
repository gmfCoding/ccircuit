#include "array.h"
#include <stdlib.h>
#include <string.h>

void	array_add(t_array *arr, void *item)
{
	if (arr->count >= arr->capacity)
	{
		arr->capacity *= 2;
		arr->elem = realloc(arr->elem, arr->capacity * arr->ofsize);
	}
	void *target = &((char *)arr->elem)[arr->ofsize * arr->count];
	memcpy(target, item, arr->ofsize);
	arr->count += 1;
}

t_array	array_init(t_asize capacity, t_asize ofsize)
{
	t_array arr = {0};
	if (capacity == 0)
		capacity = 1;
	arr.elem = malloc(capacity * ofsize);
	arr.capacity = capacity;
	arr.ofsize = ofsize;
	return (arr);
}

void	*array_get(t_array *arr, t_asize i)
{
	return (arr->elem + (i * arr->ofsize));
}

int	tarr_fullcmp(t_array *arr, void *v, void *f)
{
	return (memcmp((char *)v, (char *)f, arr->ofsize) == 0);
}

void	*array_findf(t_array *arr, void *find, t_cmpfunc func)
{
	t_asize	i = 0;

	while (i < arr->count)
	{
		void *v = array_get(arr, i);
		if (v != NULL && func(arr, v, find))
			return (v);
		i++;
	}
	return (NULL);
}

void	*array_find(t_array *arr, void *find)
{
	return (array_findf(arr, find, tarr_fullcmp));
}
