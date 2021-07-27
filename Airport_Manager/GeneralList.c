#include <stdio.h>
#include <stdlib.h>
#include "GeneralList.h"


BOOL L_init(LIST* pList)
{
	if (pList == NULL)
		return False;	
	pList->head.next = NULL;
	return True;
}



NODE* L_insert(NODE* pNode, DATA Value)
{
	NODE* tmp;

	if (!pNode)
		return NULL;

	tmp = (NODE*)malloc(sizeof(NODE));	

	if (tmp != NULL) {
		tmp->key = Value;
		tmp->next = pNode->next;
		pNode->next = tmp;
	}
	return tmp;
}


BOOL L_delete(NODE* pNode, void freeFunc(void*))
{
	NODE* tmp;
	if (!pNode)
		return False;
	tmp = pNode->next;
	if (!tmp)
		return False;

	pNode->next = tmp->next;
	if (freeFunc != NULL)
		freeFunc(tmp->key);
	free(tmp);
	return True;
}


NODE* L_find(NODE* pNode, DATA Value, int compare(const void*, const void*))
{
	NODE* tmp = pNode;

	while (tmp != NULL)
	{
		if (compare(tmp->key, Value) == 0)
			return tmp;
		tmp = tmp->next;
	}

	return NULL;
}



BOOL L_free(LIST* pList, void freeFunc(void*))
{
	NODE* tmp;
	BOOL cont = True;
	if (!pList)
		return False;

	tmp = &(pList->head);
	while (cont)
		cont = L_delete(tmp, freeFunc);

	return True;
}



int L_print(const LIST* pList, void(*print)(const void*))
{
	NODE* tmp;
	int		c = 0;

	if (!pList)
		return False;

	printf("\n");
	tmp = pList->head.next;
	while (tmp != NULL)
	{
		print(tmp->key);
		c++;
		tmp = tmp->next;
	}
	printf("\n");
	return c;
}
