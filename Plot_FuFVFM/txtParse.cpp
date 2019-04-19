#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include "txtParse.h"

using namespace std;

void txtParse::parse (vector<int> &PotentialCore, vector<int> &PotentialLambda, vector<int> &PotentialSeed, string path) {
	string IFileName, IFile, OFileName1, OFileName2, OFileName3, OFileName4, OFileName5, OFileName6, OFileName7, OFileName8, OFileName9, OFileName10, OFileName11, OFileName12;
	IFile = "Plot";
	IFileName = path + IFile + ".txt";
	ifstream Fin (IFileName); 

	string Core, Lambda, Seed, BP, NoT, CpR, HTpR, TpR, GBpR, AvgIFpR, AvgEFpR, AvgHFpR, Numof100SC, Numof50SC, Numof25SC;
	double NCore, NLambda, NSeed, NBP, NNoT, NCpR, NHTpR, NTpR, NGBpR, NAvgIFpR, NAvgEFpR, NAvgHFpR, NNumof100SC, NNumof50SC, NNumof25SC;
	vector< vector< vector<double> > > BParray, NoTarray, CpRarray, HTpRarray, TpRarray, GBpRarray, AvgIFpRarray, AvgEFpRarray, AvgHFpRarray, Numof100SCarray, Numof50SCarray, Numof25SCarray;
	vector < vector<double> > XBParray, XNoTarray, XCpRarray, XHTpRarray, XTpRarray, XGBpRarray, XAvgIFpRarray, XAvgEFpRarray, XAvgHFpRarray, XNumof100SCarray, XNumof50SCarray, XNumof25SCarray;
	vector<double> YBParray, YNoTarray, YCpRarray, YHTpRarray, YTpRarray, YGBpRarray, YAvgIFpRarray, YAvgEFpRarray, YAvgHFpRarray, YNumof100SCarray, YNumof50SCarray, YNumof25SCarray;


	for (int i = 0; i < PotentialCore.size (); i++) {
		for (int k = 0; k < PotentialLambda.size (); k++) {
			YBParray.push_back (PotentialCore[i]);
			YBParray.push_back (PotentialLambda[k]);
			for (int i = 0; i < PotentialSeed.size (); i++) {
				YBParray.push_back (-1);
			}
			XBParray.push_back (YBParray);
			YBParray.clear ();

			YNoTarray.push_back (PotentialCore[i]);
			YNoTarray.push_back (PotentialLambda[k]);
			for (int i = 0; i < PotentialSeed.size (); i++) {
				YNoTarray.push_back (-1);
			}
			XNoTarray.push_back (YNoTarray);
			YNoTarray.clear ();

			YCpRarray.push_back (PotentialCore[i]);
			YCpRarray.push_back (PotentialLambda[k]);
			for (int i = 0; i < PotentialSeed.size (); i++) {
				YCpRarray.push_back (-1);
			}
			XCpRarray.push_back (YCpRarray);
			YCpRarray.clear ();

			YHTpRarray.push_back (PotentialCore[i]);
			YHTpRarray.push_back (PotentialLambda[k]);
			for (int i = 0; i < PotentialSeed.size (); i++) {
				YHTpRarray.push_back (-1);
			}
			XHTpRarray.push_back (YHTpRarray);
			YHTpRarray.clear ();

			YTpRarray.push_back (PotentialCore[i]);
			YTpRarray.push_back (PotentialLambda[k]);
			for (int i = 0; i < PotentialSeed.size (); i++) {
				YTpRarray.push_back (-1);
			}
			XTpRarray.push_back (YTpRarray);
			YTpRarray.clear ();

			YGBpRarray.push_back (PotentialCore[i]);
			YGBpRarray.push_back (PotentialLambda[k]);
			for (int i = 0; i < PotentialSeed.size (); i++) {
				YGBpRarray.push_back (-1);
			}
			XGBpRarray.push_back (YGBpRarray);
			YGBpRarray.clear ();

			YAvgIFpRarray.push_back (PotentialCore[i]);
			YAvgIFpRarray.push_back (PotentialLambda[k]);
			for (int i = 0; i < PotentialSeed.size (); i++) {
				YAvgIFpRarray.push_back (-1);
			}
			XAvgIFpRarray.push_back (YAvgIFpRarray);
			YAvgIFpRarray.clear ();

			YAvgEFpRarray.push_back (PotentialCore[i]);
			YAvgEFpRarray.push_back (PotentialLambda[k]);
			for (int i = 0; i < PotentialSeed.size (); i++) {
				YAvgEFpRarray.push_back (-1);
			}
			XAvgEFpRarray.push_back (YAvgEFpRarray);
			YAvgEFpRarray.clear ();

			YAvgHFpRarray.push_back (PotentialCore[i]);
			YAvgHFpRarray.push_back (PotentialLambda[k]);
			for (int i = 0; i < PotentialSeed.size (); i++) {
				YAvgHFpRarray.push_back (-1);
			}
			XAvgHFpRarray.push_back (YAvgHFpRarray);
			YAvgHFpRarray.clear ();

			YNumof100SCarray.push_back (PotentialCore[i]);
			YNumof100SCarray.push_back (PotentialLambda[k]);
			for (int i = 0; i < PotentialSeed.size (); i++) {
				YNumof100SCarray.push_back (-1);
			}
			XNumof100SCarray.push_back (YNumof100SCarray);
			YNumof100SCarray.clear ();

			YNumof50SCarray.push_back (PotentialCore[i]);
			YNumof50SCarray.push_back (PotentialLambda[k]);
			for (int i = 0; i < PotentialSeed.size (); i++) {
				YNumof50SCarray.push_back (-1);
			}
			XNumof50SCarray.push_back (YNumof50SCarray);
			YNumof50SCarray.clear ();

			YNumof25SCarray.push_back (PotentialCore[i]);
			YNumof25SCarray.push_back (PotentialLambda[k]);
			for (int i = 0; i < PotentialSeed.size (); i++) {
				YNumof25SCarray.push_back (-1);
			}
			XNumof25SCarray.push_back (YNumof25SCarray);
			YNumof25SCarray.clear ();
		}
		BParray.push_back (XBParray);
		XBParray.clear ();

		NoTarray.push_back (XNoTarray);
		XNoTarray.clear ();

		CpRarray.push_back (XCpRarray);
		XCpRarray.clear ();

		HTpRarray.push_back (XHTpRarray);
		XHTpRarray.clear ();

		TpRarray.push_back (XTpRarray);
		XTpRarray.clear ();

		GBpRarray.push_back (XGBpRarray);
		XGBpRarray.clear ();

		AvgIFpRarray.push_back (XAvgIFpRarray);
		XAvgIFpRarray.clear ();

		AvgEFpRarray.push_back (XAvgEFpRarray);
		XAvgEFpRarray.clear ();

		AvgHFpRarray.push_back (XAvgHFpRarray);
		XAvgHFpRarray.clear ();

		Numof100SCarray.push_back (XNumof100SCarray);
		XNumof100SCarray.clear ();

		Numof50SCarray.push_back (XNumof50SCarray);
		XNumof50SCarray.clear ();

		Numof25SCarray.push_back (XNumof25SCarray);
		XNumof25SCarray.clear ();
	}


	while (!Fin.eof ()) {
		int CoreCnt = 0;
		int LambdaCnt = 0;
		int SeedCnt = 0;

		Fin >> Core;
		Fin >> Lambda;
		Fin >> Seed;
		Fin >> BP; 
		Fin >> NoT;
		Fin >> CpR;
		Fin >> HTpR;
		Fin >> TpR;
		Fin >> GBpR;
		Fin >> AvgIFpR;
		Fin >> AvgEFpR;
		Fin >> AvgHFpR;
		Fin >> Numof100SC;
		Fin >> Numof50SC;
		Fin >> Numof25SC;


		NCore = stof (Core);
		NLambda = stof (Lambda);
		NSeed = stof (Seed);
		NBP = stof (BP);
		NNoT = stof (NoT);
		NCpR = stof (CpR);
		NHTpR = stof (HTpR);
		NTpR = stof (TpR);
		NGBpR = stof (GBpR);
		NAvgIFpR = stof (AvgIFpR);
		NAvgEFpR = stof (AvgEFpR);
		NAvgHFpR = stof (AvgHFpR);
		NNumof100SC= stof (Numof100SC);
		NNumof50SC= stof (Numof50SC);
		NNumof25SC= stof (Numof25SC);

		for (CoreCnt = 0; CoreCnt < PotentialCore.size (); CoreCnt++) {
			if ((int) NCore == PotentialCore[CoreCnt]) break;
		}

		for (LambdaCnt = 0; LambdaCnt < PotentialLambda.size (); LambdaCnt++) {
			if (((int) NLambda / (int) NCore) == PotentialLambda[LambdaCnt]) break;
		}

		for (SeedCnt = 0; SeedCnt < PotentialSeed.size (); SeedCnt++) {
			if (NSeed == PotentialSeed[SeedCnt]) break;
		}

		
		BParray[CoreCnt][LambdaCnt].at (SeedCnt + 2) = NBP;
		NoTarray[CoreCnt][LambdaCnt].at (SeedCnt + 2) = NNoT;
		CpRarray[CoreCnt][LambdaCnt].at (SeedCnt + 2) = NCpR;
		HTpRarray[CoreCnt][LambdaCnt].at (SeedCnt + 2) = NHTpR;
		TpRarray[CoreCnt][LambdaCnt].at (SeedCnt + 2) = NTpR;
		GBpRarray[CoreCnt][LambdaCnt].at (SeedCnt + 2) = NGBpR;
		AvgIFpRarray[CoreCnt][LambdaCnt].at (SeedCnt + 2) = NAvgIFpR;
		AvgEFpRarray[CoreCnt][LambdaCnt].at (SeedCnt + 2) = NAvgEFpR;
		AvgHFpRarray[CoreCnt][LambdaCnt].at (SeedCnt + 2) = NAvgHFpR;
		Numof100SCarray[CoreCnt][LambdaCnt].at (SeedCnt + 2) = NNumof100SC;
		Numof50SCarray[CoreCnt][LambdaCnt].at (SeedCnt + 2) = NNumof50SC;
		Numof25SCarray[CoreCnt][LambdaCnt].at (SeedCnt + 2) = NNumof25SC;
	}


	int Cnt = 0;
	Cnt = BParray[0][0].size () - 2;
	for (int i = 0; i < PotentialCore.size (); i++) {
		for (int j = 0; j < PotentialLambda.size (); j++) {
			double TempBP = 0;
			double TempNoT = 0;
			double TempCpR = 0;
			double TempHTpR = 0;
			double TempTpR = 0;
			double TempGBpR = 0;
			double TempAvgIFpR = 0;
			double TempAvgEFpR = 0;
			double TempAvgHFpR = 0;
			double TempNumof100SC= 0;
			double TempNumof50SC= 0;
			double TempNumof25SC= 0;

			for (int k = 2; k < 2 + Cnt; k++) {
				TempBP += BParray[i][j][k];
				TempNoT += NoTarray[i][j][k];
				TempCpR += CpRarray[i][j][k];
				TempHTpR += HTpRarray[i][j][k];
				TempTpR += TpRarray[i][j][k];
				TempGBpR += GBpRarray[i][j][k];
				TempAvgIFpR += AvgIFpRarray[i][j][k];
				TempAvgEFpR += AvgEFpRarray[i][j][k];
				TempAvgHFpR += AvgHFpRarray[i][j][k];
				TempNumof100SC += Numof100SCarray[i][j][k];
				TempNumof50SC += Numof50SCarray[i][j][k];
				TempNumof25SC += Numof25SCarray[i][j][k];
			}

			TempBP = TempBP / Cnt;
			TempNoT = TempNoT / Cnt;
			TempCpR = TempCpR / Cnt;
			TempHTpR = TempHTpR / Cnt;
			TempTpR = TempTpR / Cnt;
			TempGBpR = TempGBpR / Cnt;
			TempAvgIFpR = TempAvgIFpR / Cnt;
			TempAvgEFpR = TempAvgEFpR / Cnt;
			TempAvgHFpR = TempAvgHFpR / Cnt;
			TempNumof100SC = TempNumof100SC/ Cnt;
			TempNumof50SC = TempNumof50SC / Cnt;
			TempNumof25SC = TempNumof25SC / Cnt;

			BParray[i][j].push_back (TempBP);
			NoTarray[i][j].push_back (TempNoT);
			CpRarray[i][j].push_back (TempCpR);
			HTpRarray[i][j].push_back (TempHTpR);
			TpRarray[i][j].push_back (TempTpR);
			GBpRarray[i][j].push_back (TempGBpR);
			AvgIFpRarray[i][j].push_back (TempAvgIFpR);
			AvgEFpRarray[i][j].push_back (TempAvgEFpR);
			AvgHFpRarray[i][j].push_back (TempAvgHFpR);
			Numof100SCarray[i][j].push_back (TempNumof100SC);
			Numof50SCarray[i][j].push_back (TempNumof50SC);
			Numof25SCarray[i][j].push_back (TempNumof25SC);
		}
	}


	OFileName1 = path + IFile + "_BP.csv";
	OFileName2 = path + IFile + "_NoT.csv";
	OFileName3 = path + IFile + "_CpR.csv";
	OFileName4 = path + IFile + "_HTpR.csv";
	OFileName5 = path + IFile + "_TpR.csv";
	OFileName6 = path + IFile + "_GBpR.csv";
	OFileName7 = path + IFile + "_AvgIFpR.csv";
	OFileName8 = path + IFile + "_AvgEFpR.csv";
	OFileName9 = path + IFile + "_AvgHFpR.csv";
	OFileName10 = path + IFile + "_Numof100SC.csv";
	OFileName11 = path + IFile + "_Numof50SC.csv";
	OFileName12 = path + IFile + "_Numof25SC.csv";
	
	ofstream Fbp (OFileName1);
	ofstream Fnot (OFileName2);
	ofstream Fcpr (OFileName3);
	ofstream Fhtpr (OFileName4);
	ofstream Ftpr (OFileName5);
	ofstream Fgbpr (OFileName6);
	ofstream Favgifpr (OFileName7);
	ofstream Favgefpr (OFileName8);
	ofstream Favghfpr (OFileName9);
	ofstream Fnumof100sc (OFileName10);
	ofstream Fnumof50sc (OFileName11);
	ofstream Fnumof25sc (OFileName12);

	string Sseed;
	for (int i = 0; i < PotentialSeed.size (); i++) {
		Sseed = Sseed + to_string (PotentialSeed[i]) + ',';
	}
	Sseed = Sseed + '\n';
	Fbp << Sseed;
	Fnot << Sseed;
	Fcpr << Sseed;
	Fhtpr << Sseed;
	Ftpr << Sseed;
	Fgbpr << Sseed;
	Favgifpr << Sseed;
	Favgefpr << Sseed;
	Favghfpr << Sseed;
	Fnumof100sc<< Sseed;
	Fnumof50sc << Sseed;
	Fnumof25sc << Sseed;

	for (int i = 0; i < BParray.size (); i++) {
		for (int j = 0; j < BParray[i].size (); j++) {
			string Sbp;
			string Snot;
			string Scpr;
			string Shtpr;
			string Stpr;
			string Sgbpr;
			string Savgifpr;
			string Savgefpr;
			string Savghfpr;
			string Snumof100sc;
			string Snumof50sc;
			string Snumof25sc;

			for (int k = 0; k < BParray[i][j].size (); k++) {
				Sbp = Sbp + to_string (BParray[i][j][k]) + ','; 
				Snot = Snot + to_string (NoTarray[i][j][k]) + ',';
				Scpr = Scpr + to_string (CpRarray[i][j][k]) + ','; 
				Shtpr = Shtpr + to_string (HTpRarray[i][j][k]) + ','; 
				Stpr = Stpr + to_string (TpRarray[i][j][k]) + ','; 
				Sgbpr = Sgbpr + to_string (GBpRarray[i][j][k]) + ',';
				Savgifpr = Savgifpr + to_string (AvgIFpRarray[i][j][k]) + ','; 
				Savgefpr = Savgefpr + to_string (AvgEFpRarray[i][j][k]) + ','; 
				Savghfpr = Savghfpr + to_string (AvgHFpRarray[i][j][k]) + ','; 
				Snumof100sc = Snumof100sc + to_string (Numof100SCarray[i][j][k]) + ','; 
				Snumof50sc = Snumof50sc + to_string (Numof50SCarray[i][j][k]) + ','; 
				Snumof25sc = Snumof25sc + to_string (Numof25SCarray[i][j][k]) + ','; 
			}

			Sbp = Sbp + '\n';
			Snot = Snot + '\n';
			Scpr = Scpr + '\n';
			Shtpr = Shtpr + '\n';
			Stpr = Stpr + '\n';
			Sgbpr = Sgbpr + '\n';
			Savgifpr = Savgifpr + '\n';
			Savgefpr = Savgefpr + '\n';
			Savghfpr = Savghfpr + '\n';
			Snumof100sc = Snumof100sc + '\n';
			Snumof50sc = Snumof50sc + '\n';
			Snumof25sc = Snumof25sc + '\n';

			Fbp << Sbp;
			Fnot << Snot;
			Fcpr << Scpr;
			Fhtpr << Shtpr;
			Ftpr << Stpr;
			Fgbpr << Sgbpr;
			Favgifpr << Savgifpr;
			Favgefpr << Savgefpr;
			Favghfpr << Savghfpr;
			Fnumof100sc << Snumof100sc;
			Fnumof50sc << Snumof50sc;
			Fnumof25sc << Snumof25sc;

		}
	}

	Fbp.close ();
	Fnot.close ();
	Fcpr.close ();
	Fhtpr.close ();
	Ftpr.close ();
	Fgbpr.close ();
	Favgifpr.close ();
	Favgefpr.close ();
	Favghfpr.close ();
	Fnumof100sc.close ();
	Fnumof50sc.close ();
	Fnumof25sc.close ();
}
