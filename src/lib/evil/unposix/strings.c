#include "strings.h"

inline int ffs(int i)
{
	for (int j = 0; j < sizeof(i)*8; j++)
	{
		if (i & (1 << j))
			return j;
	}
	return 0;
}

inline int strcasecmp(const char *s1, const char *s2)
{
	return lstrcmpiA(s1, s1);
}

inline int strcasecmp_l(const char *s1, const char *s2, locale_t locale)
{
	//CompareStringW()
}
inline int strncasecmp(const char *s1, const char *s2, size_t size)
{
	LPTSTR *n_bytes_s1 = (LPTSTR)malloc(size + 1);
	LPTSTR *n_bytes_s2 = (LPTSTR)malloc(size + 1);

	lstrcpynA(n_bytes_s1, s1, size);
	lstrcpynA(n_bytes_s2, s2, size);

	free(n_bytes_s1);
	free(n_bytes_s2);

	return lstrcmpiA(n_bytes_s1, n_bytes_s2);
}
inline int strncasecmp_l(const char *s1, const char *s2, size_t size, locale_t locale);
