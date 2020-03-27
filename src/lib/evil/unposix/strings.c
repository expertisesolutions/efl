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

inline int strncasecmp(const char *s1, const char *s2, size_t size)
{
	LPTSTR n_bytes_s1 = (LPTSTR)malloc(size + 1);
	LPTSTR n_bytes_s2 = (LPTSTR)malloc(size + 1);

	lstrcpynA(n_bytes_s1, s1, size);
	lstrcpynA(n_bytes_s2, s2, size);

	free(n_bytes_s1);
	free(n_bytes_s2);

	return lstrcmpiA(n_bytes_s1, n_bytes_s2);
}

void _nlsversionex_to_nlsversion(LPNLSVERSIONINFO nlsversion, LPNLSVERSIONINFOEX nlsversionex){
	nlsversion->dwNLSVersionInfoSize = nlsversionex->dwNLSVersionInfoSize;
	nlsversion->dwNLSVersion = nlsversionex->dwNLSVersion;
	nlsversion->dwDefinedVersion = nlsversionex->dwDefinedVersion;
}

inline int strcasecmp_l(const char *s1, const char *s2, locale_t locale)
{	
	LPNLSVERSIONINFO nls_version = (LPNLSVERSIONINFO)malloc(sizeof(NLSVERSIONINFO));
	LPNLSVERSIONINFOEX nls_versionex = (LPNLSVERSIONINFOEX)malloc(sizeof(NLSVERSIONINFO));
	nls_versionex->dwNLSVersionInfoSize = sizeof(NLSVERSIONINFO);
	_nlsversionex_to_nlsversion(nls_version,nls_versionex);

	GetNLSVersionEx(COMPARE_STRING, (LPCWSTR)locale->__names, nls_versionex);
	
	int ret = CompareStringEx((LPCWSTR)locale->__names,LINGUISTIC_IGNORECASE,(LPCWCH)s1,sizeof(s1),(LPCWCH)s2,sizeof(s2),nls_version, NULL,0);

	free(nls_version);
	free(nls_versionex);

	return ret;
}
inline int strncasecmp_l(const char *s1, const char *s2, size_t size, locale_t locale)
{
	
	LPNLSVERSIONINFO nls_version = (LPNLSVERSIONINFO)malloc(sizeof(NLSVERSIONINFO));
	LPNLSVERSIONINFOEX nls_versionex = (LPNLSVERSIONINFOEX)malloc(sizeof(NLSVERSIONINFO));
	nls_versionex->dwNLSVersionInfoSize = sizeof(NLSVERSIONINFO);
	_nlsversionex_to_nlsversion(nls_version,nls_versionex);
	
	
	GetNLSVersionEx(COMPARE_STRING, (LPCWSTR)locale->__names, nls_versionex);

	int ret = CompareStringEx((LPCWSTR)locale->__names,LINGUISTIC_IGNORECASE,(LPCWCH)s1,size,(LPCWCH)s2,size, nls_version,NULL,0);

	free(nls_version);
	free(nls_versionex);

	return ret;

}

