/*
TITLE: Assembler
AUTHOR: Alam Sai Bharath
ROLL NUMBER: 2201cs12
*/

#include <bits/stdc++.h>
using namespace std;

map<int, vector<string>> code;
vector<pair<string, string>> instructions;

map<int, int> ind;
map<string, int> labels;
vector<pair<int, string>> errors;

// Make a map with format => Name Number of operands OpCode
map<string, pair<int, int>> op;
void init()
{

	op["ldl"] = {1, 2}, op["ldc"] = {1, 0}, op["adc"] = {1, 1}, op["sp2a"] = {0, 12},

	op["stl"] = {1, 3}, op["ldnl"] = {1, 4}, op["stnl"] = {1, 5}, op["call"] = {1, 13},

	op["add"] = {0, 6}, op["sub"] = {0, 7}, op["shl"] = {0, 8}, op["return"] = {0, 14},

	op["shr"] = {0, 9}, op["adj"] = {1, 10}, op["a2sp"] = {0, 11}, op["brz"] = {1, 15},

	op["brlz"] = {1, 16}, op["br"] = {1, 17}, op["HALT"] = {0, 18}, op["data"] = {1, -1},

	op["SET"] = {1, -2};
};
vector<pair<int, int>> encodedLines; // format is opCode operand

string trim(string label)
{
	int n = label.length();
	int start = 0, end = n - 1;
	for (int i = 0; i < n; i++)
	{
		if (label[i] != ' ' && label[i] != '\t')
		{
			start = i;
			break;
		}
	}
	for (int i = n - 1; i >= 0; i--)
	{
		if (label[i] != ' ' && label[i] != '\t')
		{
			end = i;
			break;
		}
	}
	string ans = "";
	for (int i = start; i <= end; i++)
	{
		ans += label[i];
	}
	return ans;
}
int InvalidNumber(string s)
{
	for (int i = 0; i < s.size(); i++)
	{
		if (s[i] > '9' || s[i] < '0')
		{
			return 1;
		}
	}
	return 0;
}
string toHex(int num, int len)
{
	ostringstream oss;
	oss << hex << setw(len) << setfill('0') << num;
	string ans = oss.str();
	if (len < ans.size())
	{
		ans = ans.substr(ans.size() - len, len);
	}
	return ans;
}

int validLabel(string label);

void insertLabel(string label, int PC, int line)
{
	if (!validLabel(label))
	{
		errors.push_back({line, "Bogus labelname: " + label});
		return;
	}
	if (!labels.count(label))
	{ // if there or not already
		labels[label] = PC;
	}
	else
	{
		errors.push_back({line, "Duplicate label definition: " + label});
	}
}
int validLabel(string label)
{
	if (!((label[0] <= 90 && label[0] >= 65) || (label[0] <= 122 && label[0] >= 97)))
	{

		return 0;
	}
	for (int i = 1; i < label.size(); i++)
	{
		if (!((label[i] <= 90 && label[i] >= 65) || (label[i] <= 122 && label[i] >= 97) || (label[i] <= 57 && label[i] >= 48)))
		{

			return 0;
		}
	}
	return 1;
}

int decoder(string opr, int line)
{
	if (validLabel(opr) == 1)
	{
		if (labels.count(opr) == 0)
		{
			errors.push_back({line, "No such label: " + opr});
		}
		return labels[opr];
	}
	const char *start_ptr = opr.c_str();
	char *end_ptr;
	int num;
	bool issue = false;

	if (opr.size() > 1 && opr[0] == '0')
	{
		// Might be a hex / octal / binary
		switch (opr[1])
		{
		case 'b': // Base 2
			issue = InvalidNumber(opr.substr(2, opr.size() - 2));
			num = strtol(start_ptr + 2, &end_ptr, 2);
			break;
		case 'x': // Base 16
			issue = InvalidNumber(opr.substr(2, opr.size() - 2));
			num = strtol(start_ptr + 2, &end_ptr, 16);
			break;
		default: // Base 8
			issue = InvalidNumber(opr);
			num = strtol(start_ptr, &end_ptr, 8);
		}
	}
	else
	{ // Base 10
		int slice = 0;
		if (opr[0] == '+' || opr[0] == '-')
			slice++;
		issue = InvalidNumber(opr.substr(slice, opr.size() - slice));
		num = strtol(start_ptr, &end_ptr, 10);
	}
	if (issue)
	{
		errors.push_back({line, "Invalid expression: " + opr});
		return 0;
	}
	return num;
}

void firstPass(string inFile)
{
	ifstream input(inFile);
	int line = 0;
	int PC = 0;
	pair<int, string> lLabel;
	string text;

	while (getline(input, text))
	{
		line++;
		text = trim(text);
		code[PC].push_back(text);
		text = text.substr(0, text.find(';'));
		text = trim(text);
		string label = "", instr = "", opr = "";

		if (text.find(':') != -1)
		{
			int colon = text.find(':');
			label = text.substr(0, colon);
			label = trim(label);
			if (label.length())
			{
				insertLabel(label, PC, line);
				lLabel = {PC, label};
			}
			text = text.substr(colon + 1, text.length() - colon - 1);
			text = trim(text);
		}
		if (text.find(' ') != -1)
		{
			int space = text.find(' ');
			instr = text.substr(0, space);
			instr = trim(instr);
			opr = text.substr(space + 1, text.length() - space - 1);
			opr = trim(opr);
		}
		else
		{
			instr = trim(text);
		}

		if (text.size() == 0)
			continue;
		ind[PC] = line;
		if (!op.count(instr))
		{
			errors.push_back({line, "wrong Mnemonic: " + instr});
		}
		if (instr == "SET")
		{
			if (lLabel.first != PC)
			{
				errors.push_back({line, "Label doesn't exist: " + instr});
			}
			else
			{
				labels[label] = decoder(opr, line);
			}
		}

		if (opr.length() == 0 && op[instr].first != 0)
		{
			errors.push_back({line, "Missing operand: " + instr});
		}
		else if (opr.length() && !op[instr].first)
		{
			errors.push_back({line, "Unexpected operand: " + instr + " " + opr});
		}
		instructions.push_back({instr, opr});
		PC++;
	}
}

void secondPass(ofstream &logFile, ofstream &outFile, ofstream &objFile)
{
	for (int i = 0; i < instructions.size(); i++)
	{
		int line = ind[i];
		string instr = instructions[i].first;
		string opr = instructions[i].second;

		int opCode = op[instr].second;
		// if coder haven't provided operand, take it as zero and continue on the function to add in errors
		// in case of error the code stops later
		int operand = op[instr].first ? decoder(opr, line) : 0;
		if (instr == "data")
		{
			// Since data doesn't have any opcode
			// in the pdf they tell to use data as operand entirely
			opCode = operand & 0xff;
			operand >>= 8;
		}
		encodedLines.push_back({opCode, operand});
	}
	if (errors.size())
	{
		cout << "Code contains errors" << endl;
		sort(errors.begin(), errors.end());
		for (pair<int, string> it : errors)
		{
			logFile << "Line: " << it.first << " " << it.second << "\n";
		}
		return;
	}
	else
	{
		cout << "Compiled successfully" << endl;
	}

	int PC = 0;
	// Those who take offsets, here have to find the distance
	set<int> PCoffset;
	PCoffset.insert(13);
	PCoffset.insert(15);
	PCoffset.insert(16);
	PCoffset.insert(17);
	for (int PC = 0; PC < encodedLines.size(); PC++)
	{
		outFile << toHex(PC, 8) << " ";
		for (int i = 0; i < code[PC].size() - 1; i++)
		{
			string s = code[PC][i];
			outFile << "\t\t "
					<< " " << s << "\n";
			outFile << toHex(PC, 8) << " ";
		}

		int opCode = encodedLines[PC].first;
		int opr = encodedLines[PC].second;

		if (PCoffset.count(opCode))
		{
			opr -= PC + 1;
		}
		outFile << toHex(opr, 6) << toHex(opCode, 2) << " " << code[PC][code[PC].size() - 1] << "\n";

		objFile << toHex(opr, 6) << toHex(opCode, 2);
	}
}

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		cout << "Usage: ./asm file.asm" << endl;
		cout << "Where file.asm is the file to be compiled." << endl;
		return 0;
	}

	init();

	string inFile = string(argv[1]);
	firstPass(inFile);

	// Naming output files
	inFile = inFile.substr(0, inFile.find('.'));
	ofstream outFile(inFile + ".l");
	ofstream logFile(inFile + ".log");
	ofstream objFile(inFile + ".o", ios::out | ios::binary);

	secondPass(logFile, outFile, objFile);
}