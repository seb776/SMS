namespace Memory
{
	void Memset(char *data, int size)
	{
		for (int i = 0; i < size; ++i)
			data[i] = 0;
	}
}