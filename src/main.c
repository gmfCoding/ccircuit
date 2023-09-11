#include "array.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef int	t_pid;
typedef struct ic t_ic;

typedef void (*t_icfunc)(t_ic *ic);

typedef enum e_pintype t_pintype;

enum e_pintype
{
	E_OUTPUT = 1,
	E_INPUT = 2,
	E_IC = 1024,
};

struct ic
{
	t_array in;
	t_array out;

	t_icfunc *man;
};

typedef struct s_pin t_pin;

struct s_pin
{
	t_pintype type;
	int active;
	union
	{
		union
		{
			t_array pins;
		} push;
		union
		{
			t_ic *ic;
			t_pid id;
		} ic;
	};
};

struct wire
{
	t_pid in;
	t_pid out;
};

typedef struct s_resolve t_resolve;

struct s_resolve
{
	t_array ic;
	
};

typedef struct s_circuit t_circuit;
struct s_circuit
{
	t_array pins;

	t_array in_pins;
	t_array out_pins;

	t_array ic;

	t_resolve resolve;
};


void resolve_add_ic(t_resolve *resolve, t_ic *ic)
{
	if (array_find(&resolve->ic, ic) == NULL)
		array_add(&resolve->ic, ic);
}

void set_pin(t_circuit *circuit, int set, t_pid id)
{
	t_pin *pin = array_get(&circuit->pins, id);
	if (!pin)
		return;
	pin->active = set;
	if (pin->type == E_IC)
		resolve_add_ic(&circuit->resolve, pin->ic.ic);
}

void follow_pin(t_circuit *circuit, int set, t_pid id)
{
	t_pin *pin = array_get(&circuit->pins, id);
	if (!pin)
		return;
	set_pin(circuit, set, id);	
	if (pin->type == E_OUTPUT)
	{
		t_asize i = 0;
		while (i < pin->push.pins.count)
		{
			t_pid *npin = array_get(&pin->push.pins, i);
			if (npin)
				follow_pin(circuit, set, *npin);
			i++;
		}
	}
}

void resolve_in_pins(t_circuit *circ)
{
	t_asize i = 0;
	
	while (i < circ->in_pins.count)
	{
		t_pid *id = array_get(&circ->in_pins, i);
		if (id)
		{
			t_pin *pin = array_get(&circ->pins, *id);
			if (pin)
				follow_pin(circ, pin->active, *id);
		}
		i++;
	}
}

void resolve_begin(t_circuit *circuit)
{
	resolve_in_pins(circuit);
}




t_pid new_pin(t_circuit *circuit)
{
	t_pin pin = {0};
	pin.type = E_OUTPUT;
	pin.push.pins = array_init(1, sizeof(t_pid));
	
	array_add(&circuit->pins, &pin);
	return (circuit->pins.count - 1);
}

t_pid new_inpin(t_circuit *circuit)
{
	t_pid pin = new_pin(circuit);
	array_add(&circuit->in_pins, &pin);
	return pin;
}

void connect_pin(t_circuit *circuit, t_pid first, t_pid second)
{
	t_pin *f = array_get(&circuit->pins, first);
	t_pin *s = array_get(&circuit->pins, second);

	if (!f || (!s && (f->type & E_OUTPUT) == 0))
		return;
	array_add(&f->push.pins, &second);
}

int main(void)
{
	t_circuit circuit = {0};
	
	circuit.pins = array_init(1, sizeof(t_pin));
	circuit.in_pins = array_init(2, sizeof(t_pid));
	circuit.out_pins = array_init(1, sizeof(t_pid));
	circuit.ic = array_init(1, sizeof(t_pid));
	circuit.resolve.ic = array_init(1, sizeof(t_ic));
	
	t_pid first = new_inpin(&circuit);
	t_pid second = new_pin(&circuit);
	
	connect_pin(&circuit, first, second);
	t_pin *fpin = array_get(&circuit.pins, first);
	t_pin *spin = array_get(&circuit.pins, second);
	fpin->active = true;
	spin->active = false;
	resolve_begin(&circuit);
	printf("%i\n", spin->active);
	exit(0);
	return (0);
}
