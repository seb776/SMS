namespace Memory
{
	void Memset(char *data, int size)
	{
		for (int i = 0; i < size; ++i)
			data[i] = 0;
	}

	void MemCpy(char *dst, const char *src, int size)
	{
		for (int i = 0; i < size; ++i)
			dst[i] = src[i];
	}
}
