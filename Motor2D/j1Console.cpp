#include "j1Console.h"
#include "j1Window.h"
#include "j1FileSystem.h"
#include "UILabel.h"
#include "UIWindow.h"
#include "UIScrollBar.h"
#include "UIInputText.h"
#include "p2Log.h"
#include "j1Gui.h"

#define PADDING 15
#define OUTPUT_TEXT_COLOR {0,255,0,255}
#define INPUT_TEXT_COLOR {255,255,255,255}
#define ERROR_TEXT_COLOR {255,0,0,255}

// -------------------------------------------------
//                  Console
// -------------------------------------------------

j1Console::j1Console()
{
	name.create("console");
}

j1Console::~j1Console()
{
}

bool j1Console::Awake(pugi::xml_node &config)
{
	return true;
}

bool j1Console::Start()
{
	LOG("Starting Console");

	App->win->GetWindowSize(win_w, win_h);

	console_prompt = (UIWindow*)App->gui->CreateUIElement(Window, 0, 0, nullptr, win_w, 2*win_h / 5);
	console_prompt->AddListener(this);
	console_prompt->SetPriority(100);
	console_prompt->active = false;
	console_prompt->SetFillColor(0, 0, 0, 130);

	input_prompt = (UIWindow*)App->gui->CreateUIElement(Window, 0, 2 * win_h / 5, nullptr, win_w, 20);
	input_prompt->AddListener(this);
	input_prompt->SetPriority(100);
	input_prompt->active = false;
	input_prompt->SetFillColor(0, 0, 0, 130);

	input_mark = (UILabel*)App->gui->CreateUIElement(Label, PADDING-10, 0, input_prompt, 10, 20);
	input_mark->can_react = false;
	input_mark->SetText(">");
	input_mark->active = false;

	input = (UIInputText*)App->gui->CreateUIElement(InputText, PADDING, 0, input_prompt, win_w - PADDING, 20);
	input->SetDefaultText("");
	input->active = false;
	input->AddListener(this);

	scroll_labels = (UIWindow*)App->gui->CreateUIElement(Window, 0, 0, console_prompt, win_w, 1);
	scroll_labels->visible = false;
	scroll_labels->active = false;

	scroll_bar = (UIScrollBar*)App->gui->CreateUIElement(ScrollBar, win_w - 15, 0, console_prompt, 15, 154);
	scroll_bar->can_move = false;
	scroll_bar->SetBar(974, 788, 8, 154);
	scroll_bar->SetScroll(843, 330, 15, 10);
	scroll_bar->target = scroll_labels;
	scroll_bar->Active(false);

	ready = true;

	AddCommand("list", this, 0, 2, "List all commands and/or cvars. Min_args: 0 Max_args: 2 Args: commands, cvars, [searchword]");
	AddCVar("console.test", this, "test cvar");

	return true;
}

bool j1Console::PreUpdate()
{
	bool ret = true;

	return ret;
}

bool j1Console::Update(float dt)
{
	for (p2List_item<UILabel*>* label = labels.start; label; label = label->next) {
		label->data->draw_offset.y = scroll_labels->draw_offset.y;
	}

	return true;
}

bool j1Console::PostUpdate()
{
	bool ret = true;

	return ret;
}

bool j1Console::CleanUp()
{
	bool ret = true;

	return ret;
}

bool j1Console::Save(pugi::xml_node & node) const
{
	//Console save all the parameters on the config.xml

	pugi::xml_document	config_file;
	pugi::xml_node		config;

	char* buf;
	int size = App->fs->Load("config.xml", &buf);
	config_file.load_buffer(buf, size);
	RELEASE(buf);

	config = config_file.child("config");

	for (p2List_item<CVar*>* cvar = cvars.start; cvar; cvar = cvar->next) {
		p2List<p2SString> tokens;
		cvar->data->cvar_str.Tokenize('.', tokens);
		if (tokens.count() < 2) continue;
		pugi::xml_node node = config.child(tokens[0].GetString());
		if (node == NULL) node = config.append_child(tokens[0].GetString());
		pugi::xml_node parameter = node.child(tokens[1].GetString());
		if (parameter == NULL) parameter = node.append_child(tokens[1].GetString());
		cvar->data->GetCallback()->SaveCVar(tokens[1], parameter); //Each module is responsible to save their cvars
	}
	config_file.save_file("config.xml");

	return true;
}

void j1Console::UIReaction(UIElement * element, int react)
{
	Reaction reaction = static_cast<Reaction>(react);

	switch (reaction)
	{
	case MouseEnter:
		break;
	case MouseLeave:
		break;
	case RightClick:
		break;
	case LeftClick:
		break;
	case RightClickUp:
		break;
	case LeftClickUp:
		break;
	case Tab:
		break;
	case LeftArrow:
		break;
	case UpArrow:
		if (currentLabel == nullptr) {
			currentLabel = labels.end;
		}
		for (p2List_item<UILabel*>* entry = currentLabel; entry != nullptr; entry = entry->prev) {
			if (entry->data->color.r == 255 && entry->data->color.g == 255 && entry->data->color.b == 255 && entry->data->color.a == 255) {
				if (labels.count() == 1) {
					input->SetInputText(entry->data->text);
					currentLabel = entry;
				}
				else {
					if (currentLabel != entry) {
						input->SetInputText(entry->data->text);
						currentLabel = entry;
						break;
					}
				}
			}
		}
		break;
	case RightArrow:
		if (element == input) {
			bool found = false;
			for (p2List_item<Command*>* command = commands.start; command; command = command->next) {
				if (command->data->command_str.Find(input->text.c_str()) > 0) {
					found = true;
					input->SetInputText(command->data->command_str.GetString());
					break;
				}
			}
			if (!found) {
				for (p2List_item<CVar*>* cvar = cvars.start; cvar; cvar = cvar->next) {
					if (cvar->data->cvar_str.Find(input->text.c_str()) > 0) {
						input->SetInputText(cvar->data->cvar_str.GetString());
						break;
					}
				}
			}
		}
		break;
	case DownArrow:
		if (currentLabel == nullptr) {
			currentLabel = labels.start;
		}
		for (p2List_item<UILabel*>* entry = currentLabel; entry != nullptr; entry = entry->next) {
			if (entry->data->color.r == 255 && entry->data->color.g == 255 && entry->data->color.b == 255 && entry->data->color.a == 255) {
				if (labels.count() == 1) {
					input->SetInputText(entry->data->text);
					currentLabel = entry;
				}
				else {
					if (currentLabel != entry) {
						input->SetInputText(entry->data->text);
						currentLabel = entry;
						break;
					}
				}
			}
		}
		break;
	case Return:
		if (element == input) {
			UIInputText* tmp = dynamic_cast<UIInputText*>(element);
			AddText(tmp->text.c_str(), Input);
			tmp->ClearText();
		}
		break;
	default:
		break;
	}
}

void j1Console::OnCommand(p2List<p2SString>& tokens)
{
	switch (tokens.count())
	{
	case 1:
		if (tokens[0] == "list") {
			AddText("--- COMMANDS ---", Output);
			for (p2List_item<Command*>* item = commands.start; item; item = item->next){
				p2SString command_text("%s: %s.", item->data->command_str.GetString(), item->data->help.GetString());
				AddText(command_text.GetString(), Output);
			}
			AddText("     --- CVARS ---", Output);
			for (p2List_item<CVar*>* item = cvars.start; item; item = item->next) {
				p2SString cvar_text("%s: %s.", item->data->cvar_str.GetString(), item->data->help.GetString());
				AddText(cvar_text.GetString(), Output);
			}
		}
		else if (tokens[0] == "quit") {
			App->quit = true;
			ready = false;
		}
		else if (tokens[0] == "save") {
			App->SaveGame("config.xml");
		}
		break;
	case 2:
		if (tokens[0] == "list") {
			if (tokens[1] == "commands") {
				for (p2List_item<Command*>* item = commands.start; item; item = item->next) {
					p2SString command_text("%s: %s.", item->data->command_str.GetString(), item->data->help.GetString());
					AddText(command_text.GetString(), Output);
				}
			}
			else if (tokens[1] == "cvars") {
				for (p2List_item<CVar*>* item = cvars.start; item; item = item->next) {
					p2SString cvar_text("%s: %s.", item->data->cvar_str.GetString(), item->data->help.GetString());
					AddText(cvar_text.GetString(), Output);
				}
			}
			else {
				AddText("--- COMMANDS ---", Output);
				for (p2List_item<Command*>* item = commands.start; item; item = item->next) {
					if(item->data->command_str.Find(tokens[1].GetString())>0){
						p2SString command_text("%s: %s.", item->data->command_str.GetString(), item->data->help.GetString());
						AddText(command_text.GetString(), Output);
					}
				}
				AddText("     --- CVARS ---", Output);
				for (p2List_item<CVar*>* item = cvars.start; item; item = item->next) {
					if(item->data->cvar_str.Find(tokens[1].GetString())>0){
						p2SString cvar_text("%s: %s.", item->data->cvar_str.GetString(), item->data->help.GetString());
						AddText(cvar_text.GetString(), Output);
					}
				}
			}
		}
	default:
		break;
	}
}

void j1Console::OnCVar(p2List<p2SString>& tokens)
{
}

// Create a new command for the console. 
void j1Console::AddCommand(const char * command, j1Module * callback, uint min_args, uint max_args, const char* help_txt)
{
	p2SString name(command);

	name.ToLowerCase();

	uint pos;
	if(!FindCommand(name,pos))
		commands.add(new Command(name.GetString(), callback, min_args, max_args, help_txt));
}

// Create a new cvar for the console. 
void j1Console::AddCVar(const char * cvar, j1Module * callback, const char* help_txt)
{
	p2SString name(cvar);

	name.ToLowerCase();

	uint pos;
	if (!FindCommand(name, pos))
		cvars.add(new CVar(name.GetString(), callback, help_txt));
}

// Change console state to the opposite one (show or hide).
void j1Console::Activate()
{
	console_prompt->active = !console_prompt->active;
	input_prompt->active = !input_prompt->active;
	input_mark->active = !input_mark->active;
	input->active = !input->active;
	App->gui->focused_element = input;
	scroll_labels->active = !scroll_labels->active;
	for (p2List_item<UILabel*>* label = labels.start; label; label = label->next) {
		label->data->active = !label->data->active;
	}
	scroll_bar->Active(!scroll_bar->active);
}

// Add text to console. 
// txt: text console should add.
// type: input (check commands and cvars) or output (console return message).
void j1Console::AddText(const char * txt, ConsoleTextType type)
{
	if (txt[0] != '\0' && ready) {
		switch (type)
		{
		case Output: 
		{
			p2SString text(txt);
			p2List<p2SString> tokens;
			text.Tokenize('\n', tokens); 

			for (p2List_item<p2SString>* token = tokens.start; token; token = token->next) {
				if (token->data.Length() > 1) {
					UILabel* new_label = (UILabel*)App->gui->CreateUIElement(Label, PADDING*2, labels.count() * 20 - 20 * (labels.count() - console_prompt->position.h / 20), console_prompt, win_w - PADDING*2, 20);
					new_label->SetText(token->data.GetString(), OUTPUT_TEXT_COLOR);
					new_label->can_react = false;
					new_label->active = console_prompt->active;
					labels.add(new_label);

					if (new_label->position.y + 20 > console_prompt->position.h) {
						for (p2List_item<UILabel*>* label = labels.start; label; label = label->next) {
							label->data->position.y -= 20;
						}
					}
				}
			}
			tokens.clear();
		}
			break;
		case Input:
		{
			bool command_found = false;
			bool command_valid = false;
			uint command_index;

			bool cvar_found = false;
			bool cvar_valid = false;
			uint cvar_index;

			p2SString text(txt);
			p2List<p2SString> tokens;
			text.Tokenize(' ', tokens);

			tokens[0].ToLowerCase();

			command_found = FindCommand(tokens[0], command_index);
			if (command_found) {
				command_valid = CheckCommandArguments(tokens.count() - 1, command_index);
			}
			else {
				cvar_found = FindCVar(tokens[0], cvar_index);
				if (cvar_found) cvar_valid = CheckCVarArguments(tokens.count() - 1, cvar_index);
			}

			UILabel* new_label = (UILabel*)App->gui->CreateUIElement(Label, PADDING, labels.count() * 20 - 20 * (labels.count() - console_prompt->position.h / 20), console_prompt, win_w - PADDING, 20);
			if ((command_found && command_valid) || (cvar_found && cvar_valid))
				new_label->SetText(txt, INPUT_TEXT_COLOR);
			else {
				if (!command_found && !cvar_found) {
					p2SString output("Error: %s don't exist.", txt);
					new_label->SetText(output.GetString(), ERROR_TEXT_COLOR);
				}
				else if (!command_valid || !cvar_valid) {
					if(command_found)
						new_label->SetText("Error: Invalid command arguments.", ERROR_TEXT_COLOR);
					else if(cvar_found)
						new_label->SetText("Error: Invalid cvar arguments.", ERROR_TEXT_COLOR);
				}
			}
			new_label->can_react = false;
			new_label->active = console_prompt->active;
			labels.add(new_label);

			if (new_label->position.y + 20 > console_prompt->position.h) {
				for (p2List_item<UILabel*>* label = labels.start; label; label = label->next) {
					label->data->position.y -= 20;
				}
			}

			if (command_found) {
				commands[command_index]->GetCallback()->OnCommand(tokens);
			}
			else if (cvar_found) {
				cvars[cvar_index]->GetCallback()->OnCVar(tokens);
			}
		}
		break;
		default:
			break;
		}
		if (labels.count() > 20) {
			labels[0]->to_delete = true;
			labels.pop(0);
		}
	}
}

// return true if command exist and fill pos with the index on the list.
bool j1Console::FindCommand(p2SString command, uint& pos) const
{
	bool ret = false;

	int i = 0;

	for (p2List_item<Command*>* item = commands.start; item; item = item->next) {
		if (item->data->command_str == command) {
			ret = true;
			break;
		}
		i++;
	}

	if (ret == true) pos = i;

	return ret;
}

// Check if the number of arguments inputed is correct for the command.
bool j1Console::CheckCommandArguments(int num_args, int command_index) const
{
	return (num_args<=commands[command_index]->max_args && num_args>=commands[command_index]->min_args);
}

// return true if cvar exist and fill pos with the index on the list.
bool j1Console::FindCVar(p2SString cvar, uint & pos) const
{
	bool ret = false;

	int i = 0;

	for (p2List_item<CVar*>* item = cvars.start; item; item = item->next) {
		if (item->data->cvar_str == cvar) {
			ret = true;
			break;
		}
		i++;
	}

	if (ret == true) pos = i;

	return ret;
}

// Check if the number of arguments inputed is correct for the cvar.
bool j1Console::CheckCVarArguments(int num_args, int cvar_index) const
{
	return (num_args <= cvars[cvar_index]->max_args && num_args >= cvars[cvar_index]->min_args);
}

// -------------------------------------------------
//                  Command
// -------------------------------------------------

Command::Command(const char * txt, j1Module * callback, uint min_args, uint max_args, const char* help_txt) : command_str(txt), callback(callback), min_args(min_args), max_args(max_args), help(help_txt)
{
}

Command::~Command()
{
}

// Return callback module
j1Module * Command::GetCallback() const
{
	return callback;
}

// -------------------------------------------------
//                    CVar
// -------------------------------------------------

CVar::CVar(const char * txt, j1Module * callback, const char* help_txt) : cvar_str(txt), callback(callback), help(help_txt)
{
}

CVar::~CVar()
{
}

// Return callback module
j1Module * CVar::GetCallback() const
{
	return callback;
}