


int file_exists(const char * filename)
{
	FILE * file; 
    if (file = fopen(filename, "r"))
    {
        fclose(file);
        return 1;
    }
    return 0;
}