#include "FileSystem.h"
#include "NeuralNetwork.h"

int DeleteDirectory(const string& pathToDelDir, bool bDeleteSubdirectories = 1);

string wstrtTostr(const wstring &ws)
{
	using convert_type = codecvt_utf8<wchar_t>; // C++ 11 standart converter.
	wstring_convert<convert_type, wchar_t> converter;
	return converter.to_bytes(ws);
}

//////////////////////** FileSystem **//////////////////////
FileSystem::FileSystem()
{
	char path[MAX_PATH];
	GetCurrentDirectory(sizeof(path), path);

	this->currentPath = strToWstr(path);
	this->networkPath = this->currentPath;
}

FileSystem::~FileSystem()
{
}

wstring FileSystem::getCurDir()
{
	return (this->currentPath);
}

wstring FileSystem::organizeLayer(const string& networkName,
	const string& layerID)
{
	return createLayerDir(strToWstr(layerID));
}

void FileSystem::organizeNetwork(const string& networkName)
{
	createNetworkDir(strToWstr(networkName));
}

void FileSystem::createWeightFile(vector<Neuron>* neurons,
	const wstring&  path,
	const  string&  connectedWith)
{
	ofstream fout(path);
	fout << fixed;
	fout.precision(11);
	for (auto it = neurons->begin();
		it != neurons->end(); ++it)
	{
		for (auto vIt = it->getEdges()->find(connectedWith)->second.begin();
			vIt != it->getEdges()->find(connectedWith)->second.end(); ++vIt)
		{
			fout << (*vIt)->weight << ' ';
		}
		fout << '\n';
	}

	fout.close();
}

void FileSystem::deleteNetwork()
{
	DeleteDirectory(wstrtTostr(this->networkPath));
}

void FileSystem::createNetworkDir(const wstring& name)
{
	wstring path = this->currentPath;
	path.append(L"\\");
	path.append(name);
	CreateDirectoryW(path.c_str(), nullptr);
	this->networkPath = move(path); // Start string was const so we don't need to c_str().
}

wstring FileSystem::createLayerDir(const wstring& name)
{
	wstring path = this->networkPath;
	path.append(L"\\");
	path.append(name);
	CreateDirectoryW(path.c_str(), nullptr);
	return path;
}

int FileSystem::StringToWString(wstring&  ws, const string&  s)
{
	wstring wsTmp(s.begin(), s.end());

	ws = wsTmp;

	return 0;
}

wstring FileSystem::strToWstr(const string&  s)
{
	wstring_convert<codecvt_utf8_utf16<wchar_t>> converter;
	return converter.from_bytes(s);
}



int DeleteDirectory(const string& pathToDelDir, bool bDeleteSubdirectories)
{
	bool            bSubdirectory = false;       // Flag, true if subdirectories have been found.
	HANDLE          hFile;                       // Handle to directory.
	string          strFilePath;                 // Filepath.
	string          strPattern;                  // Pattern.
	WIN32_FIND_DATA FileInformation;             // File information.

	strPattern = pathToDelDir + "\\*.*";
	hFile = ::FindFirstFile(strPattern.c_str(), &FileInformation);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		do
		{
			if (FileInformation.cFileName[0] != '.')
			{
				strFilePath.erase();
				strFilePath = pathToDelDir + "\\" + FileInformation.cFileName;

				if (FileInformation.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					if (bDeleteSubdirectories)
					{
						// Delete subdirectory.
						int iRC = DeleteDirectory(strFilePath, bDeleteSubdirectories);
						if (iRC)
						{
							::FindClose(hFile);
							return iRC;
						}

					}
					else
						bSubdirectory = true;
				}
				else
				{
					// Set file attributes.
					if (::SetFileAttributes(strFilePath.c_str(), FILE_ATTRIBUTE_NORMAL) == FALSE)
						return ::GetLastError();

					// Delete file.
					if (::DeleteFile(strFilePath.c_str()) == FALSE)
						return ::GetLastError();
				}
			}
		} while (::FindNextFile(hFile, &FileInformation) != FALSE);

		// Close handle.
		::FindClose(hFile);

		DWORD dwError = ::GetLastError();
		if (dwError != ERROR_NO_MORE_FILES)
			return dwError;
		else
		{
			if (!bSubdirectory)
			{
				// Set directory attributes.
				if (::SetFileAttributes(pathToDelDir.c_str(), FILE_ATTRIBUTE_NORMAL) == FALSE)
					return ::GetLastError();

				// Delete directory.
				if (::RemoveDirectory(pathToDelDir.c_str()) == FALSE)
					return ::GetLastError();
			}
		}
	}

	return 0;
}