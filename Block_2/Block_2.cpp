#include "SHA256.h"


struct user {
	
	std::string Name = "";
	std::string Public_Key = "";
	int Balance = 0;
};

user User[1000]; //kad nereiketu linkedlisto

struct Transaction {

	std::string ID = "";
	std::string Sender = "";
	std::string Receiver = "";
	int Sum = 0;

};

struct Block {
	std::string Version = "0.1v";
	std::string Merkel;
	int TimeStamp = 0;
	int Nonce = 0;
	int DifficultyTarget = 0;
	std::string Hash;
	std::string PreviousHash;
	std::vector<Transaction> AtrinktosT; //100 sugeneruotu transakciju
	Block* next = NULL;

};

Block* n; //node
Block* h; //header		//linked listas
Block* t; //temp

std::vector<Transaction> TransPool;		//visu transakciju poolas, globalus

void UserGenerator() {

	int ran;
	char letters[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };
	for (int i = 0; i < 1000; i++){	// uzpildoma 1000 userio strukturu

		User[i].Name = "UserNum." + std::to_string(i + 1);	//kad 0 nario nebutu
		for (int y = 0; y < 32; y++){

			ran = std::round(0 + (double)rand() / RAND_MAX * 16); //nuo 0 iki 15 
			User[i].Public_Key = User[i].Public_Key + letters[ran];
		}
		ran = std::round(100 + (double)rand() / RAND_MAX * 999899); //valiuta nuo 100 iki 1.000.000
		User[i].Balance = ran;
	}
}

void TransactionGenerator() {	

	Transaction n;	//viena struktura
	
	for (int i = 0; i < 10000; i++){	//10.000 transakciju generuojama

		int ran;
		std::string u1, u2, u3;
		ran = std::round(0 + (double)rand() / RAND_MAX * 999); //paimamas vienas useris atsitiktinis kaip siuntejas
		n.Sender = User[ran].Public_Key;
		n.Sum = std::round(1 + (double)rand() / RAND_MAX * (User[ran].Balance - 1)); //sugeneruojama, kad kazkiek pinigu atidave nuo 1 iki max kiek tas asmuo turi
		User[ran].Balance = User[ran].Balance - n.Sum;	//atimami pinigai, panaudoti transakcijai
		for (int y = 0; y < 1; y++){

			ran = std::round(0 + (double)rand() / RAND_MAX * 1000);
			if (User[ran].Public_Key == u1) y--;	//generuoja atsitiktini gaveja, jei toks pat, kaip siuntejas, generuoja naujai
		}
		n.Receiver = User[ran].Public_Key;
		TransPool.push_back(n);		//sugeneruojama viena transakcija ir tada papushinama i poola
	}
	
}


std::vector<Transaction> ChoosingTransactions() {	//pasirenkamos 100 transakciju

	int ran;
	std::vector<Transaction> Atrenkamos;
	//AtrinktosT // bloko atrinktos 
	//Transpool //visos kiek yra transakciju

		for (int i = 0; i < 100; i++) {
			ran = std::round(0 + (double)rand() / RAND_MAX * (TransPool.size() - 1));	//paimama atsitiktine transakcija
			Atrenkamos.push_back(TransPool.at(ran)); //paduodama is poolo
			TransPool.erase(TransPool.begin() + ran);
		};
}


void BlockGenerator(int x) {

	for (int i = 0; i < x; i++)
	{
		n = new Block;
		n->AtrinktosT = ChoosingTransactions(); //cia pasirinks 100 atsitiktiniu transakciju vienam blokui
		n->Merkel = MerkelRoot(n->T);
		n->TimeStamp = std::time(0);
		n->DifficultyTarget = 3;
		n->Nonce = 1;
		if (i == 0) n->PreviousHash = "0";
		else n->PreviousHash = t->Hash;
		if (i == 0) h = n;
		else t->next = n;
		t = n;
		n->Hash = BlockHash();
		std::cout << i << " blokas" << std::endl;
	}
}

void Hash() {


}

void PreviousHash() {


}



int main() {

	char atsakymas;
	std::cout << "Ar reikalingas generavimas? T/N: ";
	std::cin >> atsakymas;
	if (atsakymas == 'T') {
		
		UserGenerator();
		TransactionGenerator();
		BlockGenerator(100); //100 bloku po 100 transakciju 
		std::cout << "Sugeneruota " << std::endl;
	}

	return 0;
}

