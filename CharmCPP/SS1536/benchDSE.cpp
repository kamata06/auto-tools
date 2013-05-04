#include "TestDSE.h"
#include <fstream>
#include <time.h>

string getID(int len)
{
	string alphabet = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
	string id = "";
	int val, alpha_len = alphabet.size();
	for(int i = 0; i < len; i++)
	{
		val = (int) (rand() % alpha_len);
		id +=  alphabet[val];
	}
	cout << "Rand selected ID: '" << id << "'" << endl;
	return id;
}

void benchmarkDSE(Dse09 & dse, ofstream & outfile0, ofstream & outfile1, ofstream & outfile2, int ID_string_len, int iterationCount, CharmListStr & keygenResults, CharmListStr & encryptResults, CharmListStr & decryptResults)
{
	Benchmark benchE, benchD, benchK;
    CharmList msk, mpk, pk, sk, ct;
    GT M, newM;
    ZR bf0;
    string id = getID(ID_string_len); // "somebody@example.com and other people!!!!!";
    double de_in_ms, kg_in_ms;

	dse.setup(mpk, msk);
	for(int i = 0; i < iterationCount; i++) {
		id = getID(ID_string_len);
		benchK.start();
		dse.keygen(mpk, msk, id, sk2);
		benchK.stop();
		kg_in_ms = benchK.computeTimeInMilliseconds();
	}
	cout << "Keygen avg: " << benchK.getAverage() << " ms" << endl;
    stringstream s0;
	s0 << ID_string_len << " " << benchK.getAverage() << endl;
	outfile0 << s0.str();
    keygenResults[ID_string_len] = benchK.getRawResultString();

	dse.keygen(mpk, msk, id, sk);

    //cout << "ct =\n" << ct << endl;
	for(int i = 0; i < iterationCount; i++) {
		// run enc and dec
	    M = dse.group.random(GT_t);
		benchE.start();
	    dse.encrypt(mpk, M, id, ct);
		benchE.stop();
		kg_in_ms = benchE.computeTimeInMilliseconds();

		benchD.start();
		dse.decrypt(ct, sk, newM);
		benchD.stop();
		de_in_ms = benchD.computeTimeInMilliseconds();
	}
	cout << "Encrypt avg: " << benchE.getAverage() << " ms" << endl;
    stringstream s1;
	s1 << ID_string_len << " " << benchE.getAverage() << endl;
	outfile1 << s1.str();
    keygenResults[ID_string_len] = benchE.getRawResultString();

	cout << "Decrypt avg: " << benchD.getAverage() << " ms" << endl;
    stringstream s2;
	s2 << iterationCount << " " << benchD.getAverage() << endl;
	outfile2 << s2.str();
	decryptResults[ID_string_len] = benchD.getRawResultString();

//    cout << convert_str(M) << endl;
//    cout << convert_str(newM) << endl;
    if(M == newM) {
      cout << "Successful Decryption!" << endl;
    }
    else {
      cout << "FAILED Decryption." << endl;
    }
    return;
}

int main(int argc, const char *argv[])
{
	string FIXED = "fixed", RANGE = "range";
	if(argc != 4) { cout << "Usage " << argv[0] << ": [ iterationCount => 10 ] [ ID-string => 100 ] [ 'fixed' or 'range' ]" << endl; return -1; }

	int iterationCount = atoi( argv[1] );
	int ID_string_len = atoi( argv[2] );
	string fixOrRange = string(argv[3]);
	cout << "iterationCount: " << iterationCount << endl;
	cout << "ID-string: " << ID_string_len << endl;
	cout << "measurement: " << fixOrRange << endl;

	srand(time(NULL));
	Dse09 dse;
	string filename = string(argv[0]);
	stringstream s2, s3, s4;
	ofstream outfile0, outfile1, outfile2;
	string f0 = filename + "_sym_keygen.dat";
	string f1 = filename + "_sym_encrypt.dat";
	string f2 = filename + "_sym_decrypt.dat";
	outfile0.open(f0.c_str());
	outfile1.open(f1.c_str());
	outfile2.open(f2.c_str());

	CharmListStr keygenResults, encryptResults, decryptResults;
	if(isEqual(fixOrRange, RANGE)) {
		for(int i = 2; i <= ID_string_len; i++) {
			benchmarkDSE(dse, outfile0, outfile1, outfile2, i, iterationCount, keygenResults, encryptResults, decryptResults);
		}
		s4 << decryptResults << endl;
	}
	else if(isEqual(fixOrRange, FIXED)) {
		benchmarkDSE(dse, outfile0, outfile1, outfile2, ID_string_len, iterationCount, keygenResults, encryptResults, decryptResults);
		s2 << "Raw: " << ID_string_len << " " << keygenResults[ID_string_len] << endl;
		s3 << "Raw: " << ID_string_len << " " << encryptResults[ID_string_len] << endl;
		s4 << "Raw: " << ID_string_len << " " << decryptResults[ID_string_len] << endl;
	}
	else {
		cout << "invalid option." << endl;
		return -1;
	}

	outfile0 << s2.str();
	outfile1 << s3.str();
	outfile2 << s4.str();
	outfile0.close();
	outfile1.close();
	outfile2.close();
	return 0;
}

