
#include "XMOpenCV2/contrib/contrib.hpp"
#include <kode.h>

namespace cv
{
	std::vector<std::string> Directory::GetListFiles(  const std::string& path, const std::string & exten, bool addPath )
	{
		std::vector<std::string> list;
		list.clear();
		std::string path_f = path + "/" + exten;

			KDDir *dp;
			struct KDDirent *dirp;
			if((dp = kdOpenDir (path.c_str())) == NULL)
			{
				return list;
			}

			while ((dirp = kdReadDir (dp)) != NULL)
			{
				if ( !kdStrcmp(dirp->d_name, ".") || !kdStrcmp(dirp->d_name, "..") )
				{
					continue;
				}

				std::string fpath = path_f;
				fpath += "/";
				fpath += dirp->d_name;

				KDStat st;
				kdStat ( fpath.c_str( ) , &st );

				if ( KD_ISREG ( st.st_mode ) )
				{
					if (exten.compare("*") == 0)
						list.push_back(static_cast<std::string>(dirp->d_name));
					else
						if (std::string(dirp->d_name).find(exten) != std::string::npos)
							list.push_back(static_cast<std::string>(dirp->d_name));
				}
			}
			kdCloseDir (dp);

		return list;
	}

	std::vector<std::string> Directory::GetListFolders( const std::string& path, const std::string & exten, bool addPath )
	{
		std::vector<std::string> list;
		std::string path_f = path + "/" + exten;
		list.clear();

			KDDir *dp;
			struct KDDirent *dirp;
			if((dp = kdOpenDir (path_f.c_str())) == NULL)
			{
				return list;
			}

			while ((dirp = kdReadDir (dp)) != NULL)
			{
				if ( !kdStrcmp(dirp->d_name, ".") || !kdStrcmp(dirp->d_name, "..") )
				{
					continue;
				}

				std::string fpath = path_f;
				fpath += "/";
				fpath += dirp->d_name;

				KDStat st;
				kdStat ( fpath.c_str( ) , &st );

				if ( KD_ISDIR ( st.st_mode ) )
				{
					if (exten.compare("*") == 0)
						list.push_back(static_cast<std::string>(dirp->d_name));
					else
						if (std::string(dirp->d_name).find(exten) != std::string::npos)
							list.push_back(static_cast<std::string>(dirp->d_name));
				}
			}
			kdCloseDir (dp);

		return list;
	}

	std::vector<std::string> Directory::GetListFilesR ( const std::string& path, const std::string & exten, bool addPath )
	{
		std::vector<std::string> list = Directory::GetListFiles(path, exten, addPath);

		std::vector<std::string> dirs = Directory::GetListFolders(path, exten, addPath);

		std::vector<std::string>::const_iterator it;
		for (it = dirs.begin(); it != dirs.end(); ++it)
		{
			std::vector<std::string> cl = Directory::GetListFiles(*it, exten, addPath);
			list.insert(list.end(), cl.begin(), cl.end());
		}

		return list;
	}

}
