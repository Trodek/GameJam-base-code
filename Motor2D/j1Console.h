#ifndef _J1CONSOLE_
#define _J1CONSOLE_

#include "j1Module.h"
#include "p2SString.h"
#include "p2List.h"

class UIWindow;
class UIInputText;
class UILabel;
class UIScrollBar;

enum ConsoleTextType{
	Output, Input,
};

class Command {
public:
	p2SString command_str;
	p2SString help;
	uint min_args = 0;
	uint max_args = 0;
private:
	j1Module* callback;

public:
	Command(const char* txt, j1Module* callback, uint min_args, uint max_args, const char* help_txt);
	~Command();

	j1Module* GetCallback()const;
};

class CVar {
public:
	p2SString cvar_str;
	p2SString help;
	uint min_args = 0;
	uint max_args = 1;
private:
	j1Module* callback;

public:
	CVar(const char* txt, j1Module* callback, const char* help_txt);
	~CVar();

	j1Module* GetCallback()const;
};

class j1Console :public j1Module {
public:
	j1Console();
	~j1Console();

	// Called when before render is available
	bool Awake(pugi::xml_node& config);

	// Call before first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Update Elements
	bool Update(float dt);

	// Called after all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	bool Save(pugi::xml_node& node) const;

	void UIReaction(UIElement* element, int react);

	void OnCommand(p2List<p2SString>& tokens);

	void OnCVar(p2List<p2SString>& tokens);

	void AddCommand(const char* command, j1Module* callback, uint min_args, uint max_args, const char* help_txt);

	void AddCVar(const char* cvar, j1Module* callback, const char* help_txt);

	void Activate();

	void AddText(const char* txt, ConsoleTextType type);

public:
	bool ready = false;

private:
	p2List<Command*> commands;
	p2List<CVar*> cvars;

	UIInputText* input;
	p2List<UILabel*> labels;
	UIWindow* console_prompt;
	UIWindow* input_prompt;
	UIWindow* scroll_labels;
	UIScrollBar* scroll_bar;
	UILabel* input_mark;

	uint win_w, win_h;

	bool quit = false;

	p2List_item<UILabel*>* currentLabel = nullptr;

private:
	bool FindCommand(p2SString command, uint& pos) const;
	bool CheckCommandArguments(int num_args, int command_index) const;

	bool FindCVar(p2SString cvar, uint& pos) const;
	bool CheckCVarArguments(int num_args, int command_index) const;

};


#endif // !_J1CONSOLE_
