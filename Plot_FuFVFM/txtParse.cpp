#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include "txtParse.h"

using namespace std;

void txtParse::parse (vector<int> &PotentialCore, vector<int> &PotentialLambda, vector<int> &PotentialSeed, string path) {
	string IFileName, IFile, OFileName1, OFileName2, OFileName3, OFileName4, OFileName5, OFileName6, OFileName7, OFileName8, OFileName9, OFileName10, OFileName11, OFileName12, OFileName13, OFileName14, OFileName15, OFileName16, OFileName17, OFileName18, OFileName19, OFileName20, OFileName21, OFileName22;
	IFile = "Plot";
	IFileName = path + IFile + ".txt";
	ifstream Fin (IFileName); 

	string Core, Lambda, Seed, BP, NoT, CpR, HTpR, TpR, GBpR, AvgIFpR, AvgEFpR, AvgHFpR, Numof200SC6, Numof200SC4, Numof200SC2, Numof100SC6, Numof100SC4, Numof100SC2, Numof50SC6, Numof50SC4, Numof50SC2, Numof25SC, Block400, Block100, Block40;
	double NCore, NLambda, NSeed, NBP, NNoT, NCpR, NHTpR, NTpR, NGBpR, NAvgIFpR, NAvgEFpR, NAvgHFpR, NNumof200SC6, NNumof200SC4, NNumof200SC2, NNumof100SC6, NNumof100SC4, NNumof100SC2, NNumof50SC6, NNumof50SC4, NNumof50SC2, NNumof25SC, NBlock400, NBlock100, NBlock40;
	vector< vector< vector<double> > > BParray, NoTarray, CpRarray, HTpRarray, TpRarray, GBpRarray, AvgIFpRarray, AvgEFpRarray, AvgHFpRarray, Numof200SC6array, Numof200SC4array, Numof200SC2array, Numof100SC6array, Numof100SC4array, Numof100SC2array, Numof50SC6array, Numof50SC4array, Numof50SC2array, Numof25SCarray, Block400array, Block100array, Block40array;
	vector < vector<double> > XBParray, XNoTarray, XCpRarray, XHTpRarray, XTpRarray, XGBpRarray, XAvgIFpRarray, XAvgEFpRarray, XAvgHFpRarray, XNumof200SC6array, XNumof200SC4array, XNumof200SC2array, XNumof100SC6array, XNumof100SC4array, XNumof100SC2array, XNumof50SC6array, XNumof50SC4array, XNumof50SC2array, XNumof25SCarray, XBlock400array, XBlock100array, XBlock40array;
	vector<double> YBParray, YNoTarray, YCpRarray, YHTpRarray, YTpRarray, YGBpRarray, YAvgIFpRarray, YAvgEFpRarray, YAvgHFpRarray, YNumof200SC6array, YNumof200SC4array, YNumof200SC2array, YNumof100SC6array, YNumof100SC4array, YNumof100SC2array, YNumof50SC6array, YNumof50SC4array, YNumof50SC2array, YNumof25SCarray, YBlock400array, YBlock100array, YBlock40array;


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

			YNumof200SC6array.push_back (PotentialCore[i]);
			YNumof200SC6array.push_back (PotentialLambda[k]);
			for (int i = 0; i < PotentialSeed.size (); i++) {
				YNumof200SC6array.push_back (-1);
			}
			XNumof200SC6array.push_back (YNumof200SC6array);
			YNumof200SC6array.clear ();

			YNumof200SC4array.push_back (PotentialCore[i]);
			YNumof200SC4array.push_back (PotentialLambda[k]);
			for (int i = 0; i < PotentialSeed.size (); i++) {
				YNumof200SC4array.push_back (-1);
			}
			XNumof200SC4array.push_back (YNumof200SC4array);
			YNumof200SC4array.clear ();

			YNumof200SC2array.push_back (PotentialCore[i]);
			YNumof200SC2array.push_back (PotentialLambda[k]);
			for (int i = 0; i < PotentialSeed.size (); i++) {
				YNumof200SC2array.push_back (-1);
			}
			XNumof200SC2array.push_back (YNumof200SC2array);
			YNumof200SC2array.clear ();

			YNumof100SC6array.push_back (PotentialCore[i]);
			YNumof100SC6array.push_back (PotentialLambda[k]);
			for (int i = 0; i < PotentialSeed.size (); i++) {
				YNumof100SC6array.push_back (-1);
			}
			XNumof100SC6array.push_back (YNumof100SC6array);
			YNumof100SC6array.clear ();

			YNumof100SC4array.push_back (PotentialCore[i]);
			YNumof100SC4array.push_back (PotentialLambda[k]);
			for (int i = 0; i < PotentialSeed.size (); i++) {
				YNumof100SC4array.push_back (-1);
			}
			XNumof100SC4array.push_back (YNumof100SC4array);
			YNumof100SC4array.clear ();

			YNumof100SC2array.push_back (PotentialCore[i]);
			YNumof100SC2array.push_back (PotentialLambda[k]);
			for (int i = 0; i < PotentialSeed.size (); i++) {
				YNumof100SC2array.push_back (-1);
			}
			XNumof100SC2array.push_back (YNumof100SC2array);
			YNumof100SC2array.clear ();

			YNumof50SC6array.push_back (PotentialCore[i]);
			YNumof50SC6array.push_back (PotentialLambda[k]);
			for (int i = 0; i < PotentialSeed.size (); i++) {
				YNumof50SC6array.push_back (-1);
			}
			XNumof50SC6array.push_back (YNumof50SC6array);
			YNumof50SC6array.clear ();

			YNumof50SC4array.push_back (PotentialCore[i]);
			YNumof50SC4array.push_back (PotentialLambda[k]);
			for (int i = 0; i < PotentialSeed.size (); i++) {
				YNumof50SC4array.push_back (-1);
			}
			XNumof50SC4array.push_back (YNumof50SC4array);
			YNumof50SC4array.clear ();

			YNumof50SC2array.push_back (PotentialCore[i]);
			YNumof50SC2array.push_back (PotentialLambda[k]);
			for (int i = 0; i < PotentialSeed.size (); i++) {
				YNumof50SC2array.push_back (-1);
			}
			XNumof50SC2array.push_back (YNumof50SC2array);
			YNumof50SC2array.clear ();

			YNumof25SCarray.push_back (PotentialCore[i]);
			YNumof25SCarray.push_back (PotentialLambda[k]);
			for (int i = 0; i < PotentialSeed.size (); i++) {
				YNumof25SCarray.push_back (-1);
			}
			XNumof25SCarray.push_back (YNumof25SCarray);
			YNumof25SCarray.clear ();

			YBlock400array.push_back (PotentialCore[i]);
			YBlock400array.push_back (PotentialLambda[k]);
			for (int i = 0; i < PotentialSeed.size (); i++) {
				YBlock400array.push_back (-1);
			}
			XBlock400array.push_back (YBlock400array);
			YBlock400array.clear ();

			YBlock100array.push_back (PotentialCore[i]);
			YBlock100array.push_back (PotentialLambda[k]);
			for (int i = 0; i < PotentialSeed.size (); i++) {
				YBlock100array.push_back (-1);
			}
			XBlock100array.push_back (YBlock100array);
			YBlock100array.clear ();

			YBlock40array.push_back (PotentialCore[i]);
			YBlock40array.push_back (PotentialLambda[k]);
			for (int i = 0; i < PotentialSeed.size (); i++) {
				YBlock40array.push_back (-1);
			}
			XBlock40array.push_back (YBlock40array);
			YBlock40array.clear ();
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

		Numof200SC6array.push_back (XNumof200SC6array);
		XNumof200SC6array.clear ();

		Numof200SC4array.push_back (XNumof200SC4array);
		XNumof200SC4array.clear ();

		Numof200SC2array.push_back (XNumof200SC2array);
		XNumof200SC2array.clear ();

		Numof100SC6array.push_back (XNumof100SC6array);
		XNumof100SC6array.clear ();

		Numof100SC4array.push_back (XNumof100SC4array);
		XNumof100SC4array.clear ();

		Numof100SC2array.push_back (XNumof100SC2array);
		XNumof100SC2array.clear ();

		Numof50SC6array.push_back (XNumof50SC6array);
		XNumof50SC6array.clear ();

		Numof50SC4array.push_back (XNumof50SC4array);
		XNumof50SC4array.clear ();

		Numof50SC2array.push_back (XNumof50SC2array);
		XNumof50SC2array.clear ();

		Numof25SCarray.push_back (XNumof25SCarray);
		XNumof25SCarray.clear ();

		Block400array.push_back (XBlock400array);
		XBlock400array.clear ();

		Block100array.push_back (XBlock100array);
		XBlock100array.clear ();

		Block40array.push_back (XBlock40array);
		XBlock40array.clear ();
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
		Fin >> Numof200SC6;
		Fin >> Numof200SC4;
		Fin >> Numof200SC2;
		Fin >> Numof100SC6;
		Fin >> Numof100SC4;
		Fin >> Numof100SC2;
		Fin >> Numof50SC6;
		Fin >> Numof50SC4;
		Fin >> Numof50SC2;
		Fin >> Numof25SC;
		Fin >> Block400;
		Fin >> Block100;
		Fin >> Block40;


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
		NNumof200SC6 = stof (Numof200SC6);
		NNumof200SC4 = stof (Numof200SC4);
		NNumof200SC2 = stof (Numof200SC2);
		NNumof100SC6 = stof (Numof100SC6);
		NNumof100SC4 = stof (Numof100SC4);
		NNumof100SC2 = stof (Numof100SC2);
		NNumof50SC6 = stof (Numof50SC6);
		NNumof50SC4 = stof (Numof50SC4);
		NNumof50SC2 = stof (Numof50SC2);
		NNumof25SC = stof (Numof25SC);
		NBlock400 = stof (Block400);
		NBlock100 = stof (Block100);
		NBlock40 = stof (Block40);

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
		Numof200SC6array[CoreCnt][LambdaCnt].at (SeedCnt + 2) = NNumof200SC6;
		Numof200SC4array[CoreCnt][LambdaCnt].at (SeedCnt + 2) = NNumof200SC4;
		Numof200SC2array[CoreCnt][LambdaCnt].at (SeedCnt + 2) = NNumof200SC2;
		Numof100SC6array[CoreCnt][LambdaCnt].at (SeedCnt + 2) = NNumof100SC6;
		Numof100SC4array[CoreCnt][LambdaCnt].at (SeedCnt + 2) = NNumof100SC4;
		Numof100SC2array[CoreCnt][LambdaCnt].at (SeedCnt + 2) = NNumof100SC2;
		Numof50SC6array[CoreCnt][LambdaCnt].at (SeedCnt + 2) = NNumof50SC6;
		Numof50SC4array[CoreCnt][LambdaCnt].at (SeedCnt + 2) = NNumof50SC4;
		Numof50SC2array[CoreCnt][LambdaCnt].at (SeedCnt + 2) = NNumof50SC2;
		Numof25SCarray[CoreCnt][LambdaCnt].at (SeedCnt + 2) = NNumof25SC;
		Block400array[CoreCnt][LambdaCnt].at (SeedCnt + 2) = NBlock400;
		Block100array[CoreCnt][LambdaCnt].at (SeedCnt + 2) = NBlock100;
		Block40array[CoreCnt][LambdaCnt].at (SeedCnt + 2) = NBlock40;
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
			double TempNumof200SC6= 0;
			double TempNumof200SC4= 0;
			double TempNumof200SC2= 0;
			double TempNumof100SC6= 0;
			double TempNumof100SC4= 0;
			double TempNumof100SC2= 0;
			double TempNumof50SC6= 0;
			double TempNumof50SC4= 0;
			double TempNumof50SC2= 0;
			double TempNumof25SC= 0;
			double TempBlock400= 0;
			double TempBlock100= 0;
			double TempBlock40= 0;

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
				TempNumof200SC6 += Numof200SC6array[i][j][k];
				TempNumof200SC4 += Numof200SC4array[i][j][k];
				TempNumof200SC2 += Numof200SC2array[i][j][k];
				TempNumof100SC6 += Numof100SC6array[i][j][k];
				TempNumof100SC4 += Numof100SC4array[i][j][k];
				TempNumof100SC2 += Numof100SC2array[i][j][k];
				TempNumof50SC6 += Numof50SC6array[i][j][k];
				TempNumof50SC4 += Numof50SC4array[i][j][k];
				TempNumof50SC2 += Numof50SC2array[i][j][k];
				TempNumof25SC += Numof25SCarray[i][j][k];
				TempBlock400 += Block400array[i][j][k];
				TempBlock100 += Block100array[i][j][k];
				TempBlock40 += Block40array[i][j][k];
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
			TempNumof200SC6 = TempNumof200SC6 / Cnt;
			TempNumof200SC4 = TempNumof200SC4 / Cnt;
			TempNumof200SC2 = TempNumof200SC2 / Cnt;
			TempNumof100SC6 = TempNumof100SC6 / Cnt;
			TempNumof100SC4 = TempNumof100SC4 / Cnt;
			TempNumof100SC2 = TempNumof100SC2 / Cnt;
			TempNumof50SC6 = TempNumof50SC6 / Cnt;
			TempNumof50SC4 = TempNumof50SC4 / Cnt;
			TempNumof50SC2 = TempNumof50SC2 / Cnt;
			TempNumof25SC = TempNumof25SC / Cnt;
			TempBlock400 = TempBlock400 / Cnt;
			TempBlock100 = TempBlock100 / Cnt;
			TempBlock40 = TempBlock40 / Cnt;

			BParray[i][j].push_back (TempBP);
			NoTarray[i][j].push_back (TempNoT);
			CpRarray[i][j].push_back (TempCpR);
			HTpRarray[i][j].push_back (TempHTpR);
			TpRarray[i][j].push_back (TempTpR);
			GBpRarray[i][j].push_back (TempGBpR);
			AvgIFpRarray[i][j].push_back (TempAvgIFpR);
			AvgEFpRarray[i][j].push_back (TempAvgEFpR);
			AvgHFpRarray[i][j].push_back (TempAvgHFpR);
			Numof200SC6array[i][j].push_back (TempNumof200SC6);
			Numof200SC4array[i][j].push_back (TempNumof200SC4);
			Numof200SC2array[i][j].push_back (TempNumof200SC2);
			Numof100SC6array[i][j].push_back (TempNumof100SC6);
			Numof100SC4array[i][j].push_back (TempNumof100SC4);
			Numof100SC2array[i][j].push_back (TempNumof100SC2);
			Numof50SC6array[i][j].push_back (TempNumof50SC6);
			Numof50SC4array[i][j].push_back (TempNumof50SC4);
			Numof50SC2array[i][j].push_back (TempNumof50SC2);
			Numof25SCarray[i][j].push_back (TempNumof25SC);
			Block400array[i][j].push_back (TempBlock400);
			Block100array[i][j].push_back (TempBlock100);
			Block40array[i][j].push_back (TempBlock40);
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
	OFileName10 = path + IFile + "_Numof200SC6.csv";
	OFileName11 = path + IFile + "_Numof200SC4.csv";
	OFileName12 = path + IFile + "_Numof200SC2.csv";
	OFileName13 = path + IFile + "_Numof100SC6.csv";
	OFileName14 = path + IFile + "_Numof100SC4.csv";
	OFileName15 = path + IFile + "_Numof100SC2.csv";
	OFileName16 = path + IFile + "_Numof50SC6.csv";
	OFileName17 = path + IFile + "_Numof50SC4.csv";
	OFileName18 = path + IFile + "_Numof50SC2.csv";
	OFileName19 = path + IFile + "_Numof25SC.csv";
	OFileName20 = path + IFile + "_Block400.csv";
	OFileName20 = path + IFile + "_Block100.csv";
	OFileName22 = path + IFile + "_Block40.csv";
	
	ofstream Fbp (OFileName1);
	ofstream Fnot (OFileName2);
	ofstream Fcpr (OFileName3);
	ofstream Fhtpr (OFileName4);
	ofstream Ftpr (OFileName5);
	ofstream Fgbpr (OFileName6);
	ofstream Favgifpr (OFileName7);
	ofstream Favgefpr (OFileName8);
	ofstream Favghfpr (OFileName9);
	ofstream Fnumof200sc6 (OFileName10);
	ofstream Fnumof200sc4 (OFileName11);
	ofstream Fnumof200sc2 (OFileName12);
	ofstream Fnumof100sc6 (OFileName13);
	ofstream Fnumof100sc4 (OFileName14);
	ofstream Fnumof100sc2 (OFileName15);
	ofstream Fnumof50sc6 (OFileName16);
	ofstream Fnumof50sc4 (OFileName17);
	ofstream Fnumof50sc2 (OFileName18);
	ofstream Fnumof25sc (OFileName19);
	ofstream Fblock400 (OFileName20);
	ofstream Fblock100 (OFileName21);
	ofstream Fblock40 (OFileName22);

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
	Fnumof200sc6 << Sseed;
	Fnumof200sc4 << Sseed;
	Fnumof200sc2 << Sseed;
	Fnumof100sc6 << Sseed;
	Fnumof100sc4 << Sseed;
	Fnumof100sc2 << Sseed;
	Fnumof50sc6 << Sseed;
	Fnumof50sc4 << Sseed;
	Fnumof50sc2 << Sseed;
	Fnumof25sc << Sseed;
	Fblock400<< Sseed;
	Fblock100<< Sseed;
	Fblock40<< Sseed;

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
			string Snumof200sc6;
			string Snumof200sc4;
			string Snumof200sc2;
			string Snumof100sc6;
			string Snumof100sc4;
			string Snumof100sc2;
			string Snumof50sc6;
			string Snumof50sc4;
			string Snumof50sc2;
			string Snumof25sc;
			string Sblock400;
			string Sblock100;
			string Sblock40;

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
				Snumof200sc6 = Snumof200sc6 + to_string (Numof200SC6array[i][j][k]) + ','; 
				Snumof200sc4 = Snumof200sc4 + to_string (Numof200SC4array[i][j][k]) + ','; 
				Snumof200sc2 = Snumof200sc2 + to_string (Numof200SC2array[i][j][k]) + ','; 
				Snumof100sc6 = Snumof100sc6 + to_string (Numof100SC6array[i][j][k]) + ','; 
				Snumof100sc4 = Snumof100sc4 + to_string (Numof100SC4array[i][j][k]) + ','; 
				Snumof100sc2 = Snumof100sc2 + to_string (Numof100SC2array[i][j][k]) + ','; 
				Snumof50sc6 = Snumof50sc6 + to_string (Numof50SC6array[i][j][k]) + ','; 
				Snumof50sc4 = Snumof50sc4 + to_string (Numof50SC4array[i][j][k]) + ','; 
				Snumof50sc2 = Snumof50sc2 + to_string (Numof50SC2array[i][j][k]) + ','; 
				Snumof25sc = Snumof25sc + to_string (Numof25SCarray[i][j][k]) + ','; 
				Sblock400 = Sblock400 + to_string (Block400array[i][j][k]) + ',';
				Sblock100 = Sblock100 + to_string (Block100array[i][j][k]) + ',';
				Sblock40 = Sblock40 + to_string (Block40array[i][j][k]) + ',';
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
			Snumof200sc6 = Snumof200sc6 + '\n';
			Snumof200sc4 = Snumof200sc4 + '\n';
			Snumof200sc2 = Snumof200sc2 + '\n';
			Snumof100sc6 = Snumof100sc6 + '\n';
			Snumof100sc4 = Snumof100sc4 + '\n';
			Snumof100sc2 = Snumof100sc2 + '\n';
			Snumof50sc6 = Snumof50sc6 + '\n';
			Snumof50sc4 = Snumof50sc4 + '\n';
			Snumof50sc2 = Snumof50sc2 + '\n';
			Snumof25sc = Snumof25sc + '\n';
			Sblock400 = Sblock400 + '\n';
			Sblock100 = Sblock100 + '\n';
			Sblock40 = Sblock40 + '\n';

			Fbp << Sbp;
			Fnot << Snot;
			Fcpr << Scpr;
			Fhtpr << Shtpr;
			Ftpr << Stpr;
			Fgbpr << Sgbpr;
			Favgifpr << Savgifpr;
			Favgefpr << Savgefpr;
			Favghfpr << Savghfpr;
			Fnumof200sc6 << Snumof200sc6;
			Fnumof200sc4 << Snumof200sc4;
			Fnumof200sc2 << Snumof200sc2;
			Fnumof100sc6 << Snumof100sc6;
			Fnumof100sc4 << Snumof100sc4;
			Fnumof100sc2 << Snumof100sc2;
			Fnumof50sc6 << Snumof50sc6;
			Fnumof50sc4 << Snumof50sc4;
			Fnumof50sc2 << Snumof50sc2;
			Fnumof25sc << Snumof25sc;
			Fblock400 << Sblock400;
			Fblock100 << Sblock100;
			Fblock40 << Sblock40;

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
	Fnumof200sc6.close ();
	Fnumof200sc4.close ();
	Fnumof200sc2.close ();
	Fnumof100sc6.close ();
	Fnumof100sc4.close ();
	Fnumof100sc2.close ();
	Fnumof50sc6.close ();
	Fnumof50sc4.close ();
	Fnumof50sc2.close ();
	Fnumof25sc.close ();
	Fblock400.close ();
	Fblock100.close ();
	Fblock40.close ();
}
