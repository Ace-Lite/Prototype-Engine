#include <vector>

class SaveFile {
	// All Lua stuff will be saved here!
	int lua_datatable;
	int engineversion;

	SaveFile();
	~SaveFile();
};

class SaveManager {
public:

	int current_savefile = 0;

	std::vector<SaveFile*> savefiles;

	void allocateSave();

	void loadSave();
	
	void deleteSave();
};