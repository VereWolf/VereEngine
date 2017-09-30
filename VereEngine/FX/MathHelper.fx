
int Wrap(int i, int size)
{
	while (i < 0) { i += size; }
	while (i >= size) { i -= size; }

	return i;
}