//
//  neuralnet.cpp
//  yothello
//
//  Created by 吉田拓真 on 2018/06/26.
//  Copyright © 2018年 吉田拓真. All rights reserved.
//

#include "neuralnet.hpp"
#include "math.hpp"


Neuralnet::Neuralnet()
:loaded(false),
layers(0),
nodes(0, 0),
bia(0, vector<double>(0, 0)),
wei(0, vector< vector<double> >(0, vector<double>(0, 0))),
max_nod(0)
{
    
}

Neuralnet::~Neuralnet()
{
}

void Neuralnet::set_network(vector<int> &n)
{
    nodes = n;
    layers = nodes.size();
    bia.resize(layers - 1);
    wei.resize(layers - 1);
    for (int i = 0; i < layers - 1; ++i) {
        bia[i].resize(nodes[i + 1]);
        wei[i].resize(nodes[i + 1]);
        for (int j = 0; j < nodes[i + 1]; ++j) {
            wei[i][j].resize(nodes[i]);
        }
    }
}

void Neuralnet::initialize_bw()
{
    std::random_device rnd;
    std::mt19937 mt(rnd());
    normal_distribution<double> distribution(0.0, 0.1);
    for (int i = 0; i < layers - 1; ++i) {
        for (int j = 0; j < nodes[i + 1]; ++j) {
            bia[i][j] = 0.;
            //bia[i][j] = distribution(mt);
            for (int k = 0; k < nodes[i]; ++k) {
                wei[i][j][k] = distribution(mt) / nodes[i];
            }
        }
    }
}

//vector<double> Neuralnet::Forward(const vector<double> &x) const
//{
//    vector<vector<double>> sig;
//    const int lo_layers(layers);
//    auto itr_nodes = nodes.begin();
//    auto itr_wei = wei.begin();
//    auto itr_bia = bia.begin();
//    sig.resize(lo_layers);
//    for (int i = 0;i < lo_layers;++i) {
//        sig[i].resize(itr_nodes[i]);
//    }
//    for (int j = 0;j < itr_nodes[0];++j) {
//        sig[0][j] = x[j];
//    }
//    for (int i = 1;i < lo_layers - 1;++i) {
//        for (int n1 = 0;n1 < itr_nodes[i];++n1) {
//            sig[i][n1] = itr_bia[i - 1][n1];
//            for (int n0 = 0;n0 < itr_nodes[i - 1];++n0) {
//                sig[i][n1] = sig[i][n1] + itr_wei[i - 1][n1][n0] * sig[i - 1][n0];
//            }
//            sig[i][n1] = max(0.0, sig[i][n1]);
//        }
//    }
//    int i(lo_layers - 1);
//    for (int n1 = 0;n1 < itr_nodes[i];++n1) {
//        sig[i][n1] = itr_bia[i - 1][n1];
//        for (int n0 = 0;n0 < itr_nodes[i - 1];++n0) {
//            sig[i][n1] = sig[i][n1] + itr_wei[i - 1][n1][n0] * sig[i - 1][n0];
//        }
//    }
//    return sig[lo_layers - 1];
//}

vector<double> Neuralnet::Forward(const vector<double> &x) const
{
    vector< vector<double> > sig;
    const int lo_layers(layers);
    auto itr_nodes = nodes.begin();
    auto itr_wei = wei.begin();
    auto itr_bia = bia.begin();
    sig.resize(lo_layers);
    for (int i = 0; i < lo_layers; ++i) {
        sig[i].resize(itr_nodes[i]);
    }
    for (int j = 0; j < itr_nodes[0]; ++j) {
        sig[0][j] = x[j];
    }
    for (int i(0); i < lo_layers - 2; ++i) {
        for (int n1(0); n1 < itr_nodes[i + 1]; ++n1) {
            sig[i + 1][n1] = itr_bia[i][n1];
        }
        for (int n0(0); n0 < itr_nodes[i]; ++n0) {
            double lo_sig = sig[i][n0];
            if (lo_sig != 0) {
                for (int n1(0); n1 < itr_nodes[i + 1]; ++n1) {
                    sig[i + 1][n1] += (lo_sig * itr_wei[i][n1][n0]);
                }
            }
        }
        for (int n1(0); n1 < itr_nodes[i + 1]; ++n1) {
            sig[i + 1][n1] = max(0.0, sig[i + 1][n1]);
        }
    }
    
    int i(lo_layers - 2);
    for (int n1(0); n1 < itr_nodes[i + 1]; ++n1) {
        sig[i + 1][n1] = itr_bia[i][n1];
    }
    for (int n0(0); n0 < itr_nodes[i]; ++n0) {
        double lo_sig = sig[i][n0];
        if (lo_sig != 0) {
            for (int n1(0); n1 < itr_nodes[i + 1]; ++n1) {
                sig[i + 1][n1] += (lo_sig * itr_wei[i][n1][n0]);
            }
        }
    }
    
    //for (int j(0); j < lo_layers; ++j) {
    //    for (int i(0); i < itr_nodes[j]; ++i) {
    //        cout << sig[j][i] << ",";
    //    }
    //    cout << endl;
    //}
    
    return sig[lo_layers - 1];
}

//vector<double> Neuralnet::ForwardM(const vector<double> &x) const
//{
//    vector< vector<double> > sig;
//    const int lo_layers(layers);
//    auto itr_nodes = nodes.begin();
//    auto itr_wei = wei.begin();
//    auto itr_bia = bia.begin();
//    sig.resize(lo_layers);
//    for (int i = 0; i < lo_layers; ++i) {
//        sig[i].resize(itr_nodes[i]);
//    }
//    for (int j = 0; j < itr_nodes[0]; ++j) {
//        sig[0][j] = x[j];
//    }
//    for (int i(0); i < lo_layers - 2; ++i) {
//        for (int n1(0); n1 < itr_nodes[i + 1]; ++n1) {
//            sig[i + 1][n1] = itr_bia[i][n1];
//        }
//        parallel_for(0, itr_nodes[i + 1], [&sig, &itr_nodes, &itr_wei, i](int n1) {
//            for (int n0(0); n0 < itr_nodes[i]; ++n0) {
//                sig[i + 1][n1] += (sig[i][n0] * itr_wei[i][n1][n0]);
//            }
//        });
//        for (int n1(0); n1 < itr_nodes[i + 1]; ++n1) {
//            sig[i + 1][n1] = max(0.0, sig[i + 1][n1]);
//        }
//    }
//
//    int i(lo_layers - 2);
//    for (int n1(0); n1 < itr_nodes[i + 1]; ++n1) {
//        sig[i + 1][n1] = itr_bia[i][n1];
//    }
//    for (int n0(0); n0 < itr_nodes[i]; ++n0) {
//        double lo_sig = sig[i][n0];
//        if (lo_sig != 0) {
//            for (int n1(0); n1 < itr_nodes[i + 1]; ++n1) {
//                sig[i + 1][n1] += (lo_sig * itr_wei[i][n1][n0]);
//            }
//        }
//    }
//
//    //for (int j(0); j < lo_layers; ++j) {
//    //    for (int i(0); i < itr_nodes[j]; ++i) {
//    //        cout << sig[j][i] << ",";
//    //    }
//    //    cout << endl;
//    //}
//
//    return sig[lo_layers - 1];
//}


//vector<double> Neuralnet::Forward(const vector<double> &x) const
//{
//    vector<vector<double>> sig;
//    const int lo_layers(layers);
//    vector<int> lo_nodes(nodes);
//    auto itr_nodes = nodes.begin();
//    vector<vector<vector<double>>> lo_wei(wei);
//    vector<vector<double>> lo_bia(bia);
//    sig.resize(lo_layers);
//    for (int i = 0;i < lo_layers;++i) {
//        sig[i].resize(lo_nodes[i]);
//    }
//    for (int j = 0;j < lo_nodes[0];++j) {
//        sig[0][j] = x[j];
//    }
//    for (int i = 1;i < lo_layers - 1;++i) {
//        for (int n1 = 0;n1 < lo_nodes[i];++n1) {
//            sig[i][n1] = lo_bia[i - 1][n1];
//            for (int n0 = 0;n0 < lo_nodes[i - 1];++n0) {
//                sig[i][n1] = sig[i][n1] + lo_wei[i - 1][n1][n0] * sig[i - 1][n0];
//            }
//            sig[i][n1] = max(0.0, sig[i][n1]);
//        }
//    }
//    int i(lo_layers - 1);
//    for (int n1 = 0;n1 < lo_nodes[i];++n1) {
//        sig[i][n1] = lo_bia[i - 1][n1];
//        for (int n0 = 0;n0 < lo_nodes[i - 1];++n0) {
//            sig[i][n1] = sig[i][n1] + lo_wei[i - 1][n1][n0] * sig[i - 1][n0];
//        }
//    }
//    return sig[lo_layers - 1];
//}

//void Neuralnet::Backward(vector<vector<double>> &SdEdb, vector<vector<vector<double>>> &SdEdw, const vector<double> &x, const vector<double> &y) const
//{
//    vector<vector<double>> sig;
//    vector<vector<double>> del;
//    vector<vector<double>> dEdb;
//    vector<vector<vector<double>>> dEdw;
//    sig.resize(layers);
//    del.resize(layers - 1);
//    dEdb.resize(layers - 1);
//    dEdw.resize(layers - 1);
//    for (int i = 0;i < layers - 1;++i) {
//        del[i].resize(nodes[i + 1]);
//        dEdw[i].resize(nodes[i + 1]);
//        dEdb[i].resize(nodes[i + 1]);
//        for (int j = 0;j < nodes[i + 1];++j) {
//            dEdw[i][j].resize(nodes[i]);
//        }
//    }
//    for (int i = 0;i < layers;++i) {
//        sig[i].resize(nodes[i]);
//    }
//    //����
//    for (int j = 0;j < nodes[0];++j) {
//        sig[0][j] = x[j];
//    }
//
//    //�o��
//    for (int i = 1;i < layers - 1;++i) {
//        for (int n1 = 0;n1 < nodes[i];++n1) {
//            sig[i][n1] = bia[i - 1][n1];
//            for (int n0 = 0;n0 < nodes[i - 1];++n0) {
//                sig[i][n1] = sig[i][n1] + wei[i - 1][n1][n0] * sig[i - 1][n0];
//            }
//            sig[i][n1] = max(0.0, sig[i][n1]);
//        }
//    }
//    for (int n1 = 0;n1 < nodes[layers - 1];++n1) {
//        sig[layers - 1][n1] = bia[layers - 2][n1];
//        for (int n0 = 0;n0 < nodes[layers - 2];++n0) {
//            sig[layers - 1][n1] = sig[layers - 1][n1] + wei[layers - 2][n1][n0] * sig[layers - 2][n0];
//        }
//    }
//
//
//    //�덷�t�`�dppp
//    for (int n1 = 0;n1 < nodes[layers - 1];++n1) {
//        del[layers - 2][n1] = sig[layers - 1][n1] - y[n1];
//        dEdb[layers - 2][n1] = del[layers - 2][n1];
//        SdEdb[layers - 2][n1] += dEdb[layers - 2][n1];
//        for (int n0 = 0;n0 < nodes[layers - 2];++n0) {
//            dEdw[layers - 2][n1][n0] = del[layers - 2][n1] * sig[layers - 2][n0];
//            SdEdw[layers - 2][n1][n0] += dEdw[layers - 2][n1][n0];
//        }
//    }
//    for (int i = layers - 3;i >= 0;--i) {
//        for (int n1 = 0;n1 < nodes[i + 1];++n1) {
//            del[i][n1] = 0;
//            if (sig[i + 1][n1] != 0) {
//                for (int n2 = 0;n2 < nodes[i + 2];++n2) {
//                    del[i][n1] = del[i][n1] + del[i + 1][n2] * wei[i + 1][n2][n1];
//                }
//                dEdb[i][n1] = del[i][n1];
//                SdEdb[i][n1] += dEdb[i][n1];
//                for (int n0 = 0;n0 < nodes[i];++n0) {
//                    dEdw[i][n1][n0] = del[i][n1] * sig[i][n0];
//                    SdEdw[i][n1][n0] += dEdw[i][n1][n0];
//                }
//            }
//        }
//    }
//}

void Neuralnet::Backward(vector< vector<double> > &SdEdb, vector<vector<vector<double> > > &SdEdw, const vector<double> &x, const vector<double> &y) const
{
    vector<vector<double> > del;
    vector<vector<double> > dEdb;
    vector<vector<vector<double> > > dEdw;
    del.resize(layers - 1);
    dEdb.resize(layers - 1);
    dEdw.resize(layers - 1);
    for (int i = 0; i < layers - 1; ++i) {
        del[i].resize(nodes[i + 1]);
        dEdw[i].resize(nodes[i + 1]);
        dEdb[i].resize(nodes[i + 1]);
        for (int j = 0; j < nodes[i + 1]; ++j) {
            dEdw[i][j].resize(nodes[i]);
        }
    }
    
    //����
    vector<vector<double> > sig;
    const int lo_layers(layers);
    auto itr_nodes = nodes.begin();
    auto itr_wei = wei.begin();
    auto itr_bia = bia.begin();
    sig.resize(lo_layers);
    for (int i = 0; i < lo_layers; ++i) {
        sig[i].resize(itr_nodes[i]);
    }
    for (int j = 0; j < itr_nodes[0]; ++j) {
        sig[0][j] = x[j];
    }
    for (int i(0); i < lo_layers - 2; ++i) {
        for (int n1(0); n1 < itr_nodes[i + 1]; ++n1) {
            sig[i + 1][n1] = itr_bia[i][n1];
        }
        for (int n0(0); n0 < itr_nodes[i]; ++n0) {
            double lo_sig = sig[i][n0];
            if (abs(lo_sig) > 1e-10) {
                for (int n1(0); n1 < itr_nodes[i + 1]; ++n1) {
                    sig[i + 1][n1] += (lo_sig * itr_wei[i][n1][n0]);
                }
            }
        }
        for (int n1(0); n1 < itr_nodes[i + 1]; ++n1) {
            sig[i + 1][n1] = max(0.0, sig[i + 1][n1]);
        }
    }
    
    int i(lo_layers - 2);
    for (int n1(0); n1 < itr_nodes[i + 1]; ++n1) {
        sig[i + 1][n1] = itr_bia[i][n1];
    }
    for (int n0(0); n0 < itr_nodes[i]; ++n0) {
        double lo_sig = sig[i][n0];
        if (abs(lo_sig) > 1e-10) {
            for (int n1(0); n1 < itr_nodes[i + 1]; ++n1) {
                sig[i + 1][n1] += (lo_sig * itr_wei[i][n1][n0]);
            }
        }
    }
    
    
    //�덷�t�`�dppp
    for (int n1 = 0; n1 < nodes[layers - 1]; ++n1) {
        del[layers - 2][n1] = sig[layers - 1][n1] - y[n1];
        dEdb[layers - 2][n1] = del[layers - 2][n1];
        SdEdb[layers - 2][n1] += dEdb[layers - 2][n1];
        for (int n0 = 0; n0 < nodes[layers - 2]; ++n0) {
            dEdw[layers - 2][n1][n0] = del[layers - 2][n1] * sig[layers - 2][n0];
            SdEdw[layers - 2][n1][n0] += dEdw[layers - 2][n1][n0];
        }
    }
    for (int i = layers - 3; i >= 0; --i) {
        for (int n1 = 0; n1 < nodes[i + 1]; ++n1) {
            del[i][n1] = 0;
            if (abs(sig[i + 1][n1]) > 1e-10) {
                for (int n2 = 0; n2 < nodes[i + 2]; ++n2) {
                    del[i][n1] += del[i + 1][n2] * wei[i + 1][n2][n1];
                }
                // dEdb[i][n1] = del[i][n1] + itr_bia[i][n1] / nodes[i + 1] / 1000.;
                dEdb[i][n1] = del[i][n1] + itr_bia[i][n1];
                SdEdb[i][n1] += dEdb[i][n1];
                for (int n0 = 0; n0 < nodes[i]; ++n0) {
                    // dEdw[i][n1][n0] = del[i][n1] * sig[i][n0] + itr_wei[i][n1][n0] / nodes[i] / nodes[i + 1] / 1000.;
                    dEdw[i][n1][n0] = del[i][n1] * sig[i][n0];
                    SdEdw[i][n1][n0] += dEdw[i][n1][n0];
                }
            }
        }
    }
}

void Neuralnet::save(string filename) const
{
    std::ofstream writing_file;
    writing_file.open(filename, std::ios::out);
    for (int i = 0; i < layers; ++i) {
        writing_file << nodes[i] << ",";
    }
    writing_file << std::endl;
    
    
    for (int i(0); i < layers - 1; ++i) {
        for (int n1(0); n1 < nodes[i + 1]; ++n1) {
            writing_file << bia[i][n1] << ",";
        }
        writing_file << std::endl;
    }
    for (int i(0); i < layers - 1; ++i) {
        for (int n1(0); n1 < nodes[i + 1]; ++n1) {
            for (int n0 = 0; n0 < nodes[i]; ++n0) {
                writing_file << wei[i][n1][n0] << ",";
            }
            writing_file << std::endl;
        }
    }
    writing_file.close();
}

void Neuralnet::read_network(string filename)
{
	ifstream ifs(filename);
    if (!ifs) {
		cout << "NO NETFILE" << endl;
        return;
    }
    vector<int> hp(0, 0);
    string str;
    getline(ifs, str);
    string token;
    std::istringstream stream(str);
    while (getline(stream, token, ',')) {
        hp.emplace_back(stoi(token));
    }
    
    set_network(hp);
    
    ifs.close();
}

void Neuralnet::load(string filename)
{
    ifstream ifs(filename);
    if (!ifs) {
        return;
    }
    vector<int> hp(0, 0);
    string str;
    getline(ifs, str);
    string token;
    std::istringstream stream(str);
    while (getline(stream, token, ',')) {
        hp.emplace_back(stoi(token));
    }
    
    set_network(hp);
    
    int i(0);
    int n1(0);
    while (i < layers - 1) {
        getline(ifs, str);
        n1 = 0;
        istringstream stream(str);
        while (getline(stream, token, ',')) {
            bia[i][n1] = stod(token);
            ++n1;
        }
        ++i;
    }
    
    i = 0;
    int n0(0);
    while (i < layers - 1) {
        n1 = 0;
        while (n1 < nodes[i + 1]) {
            getline(ifs, str);
            n0 = 0;
            std::istringstream stream(str);
            while (getline(stream, token, ',')) {
                wei[i][n1][n0] = std::stod(token);
                ++n0;
            }
            ++n1;
        }
        ++i;
    }
    
    ifs.close();
	loaded = true;
}

//AMP

//void Neuralnet::Set_networkG(vector<int> &nod0)
//{
//    nod = nod0;
//    lay = nod.size();
//    for (int i = 0; i < lay; ++i) {
//        max_nod = max(max_nod, nod0[i]);
//    }
//    exb = { lay - 1, max_nod };
//    exw = { lay - 1, max_nod, max_nod };
//    parr_bia = new array<double, 2>(exb, Acs.get_default_view());
//    parr_wei = new array<double, 3>(exw, Acs.get_default_view());
//}
//
//void Neuralnet::Initialize_bia_weiG()
//{
//    random_device rnd;
//    mt19937 mt(rnd());
//    normal_distribution<double> distribution(0.0, 1.0);
//    array_view<double, 2> arrV_bia = *parr_bia;
//    array_view<double, 3> arrV_wei = *parr_wei;
//
//    for (int i = 0; i < lay - 1; ++i) {
//        for (int j = 0; j < nod[i + 1]; ++j) {
//            arrV_bia[i][j] = 0;
//            //bia[i][j] = distribution(mt);
//            for (int k = 0; k < nod[i]; ++k) {
//                arrV_wei[i][j][k] = distribution(mt) / pow(double(nod[i]), 0.5);
//            }
//        }
//    }
//}

//vector<double> Neuralnet::ForwardG(const vector<double> &x)  const
//{
//    const int lo_lay(lay);
//    auto itr_nod = nod.begin();
//    array<double, 2> *parr_sig;
//    parr_sig = new array<double, 2>(exb, Acs.get_default_view());
//    array_view<double, 2> arrV_sig = *parr_sig;
//    array_view<double, 2> arrV_bia = *parr_bia;
//    array_view<double, 3> arrV_wei = *parr_wei;
//    for (int n = 0; n < itr_nod[0]; ++n) {
//        arrV_sig(0, n) = x[n];
//    }
//    for (int i(0); i < lo_lay - 2; ++i) {
//        int in0(itr_nod[i]);
//        int in1(itr_nod[i + 1]);
//        extent<1> ex(in1);
//        parallel_for_each(Acs.get_default_view(), ex, [=](index<1> idx) restrict(amp) {
//            arrV_sig(i + 1, idx[0]) = arrV_bia(i, idx[0]);
//            for (int j(0); j < in0; ++j) {
//                arrV_sig(i + 1, idx[0]) += arrV_sig(i, j) * arrV_wei(i, idx[0], j);
//            }
//            arrV_sig(i + 1, idx[0]) = max(0.0, arrV_sig(i + 1, idx[0]));
//        });
//    }
//
//    int inf0(itr_nod[lo_lay - 2]);
//    int inf(itr_nod[lo_lay - 1]);
//    //extent<1> ex(inf);
//    //parallel_for_each(Acs.get_default_view(), ex, [=](index<1> idx) restrict(amp) {
//    //    arrV_sig(lo_lay - 1, idx[0]) = arrV_bia(lo_lay - 2, idx[0]);
//    //    for (int j(0); j < inf0; ++j) {
//    //        arrV_sig(lo_lay - 1, idx[0]) += arrV_sig(lo_lay - 2, j) * arrV_wei(lo_lay - 2, idx[0], j);
//    //    }
//    //});
//
//    for (int i(0); i < inf; ++i) {
//        arrV_sig(lo_lay - 1, i) = arrV_bia(lo_lay - 2, i);
//        for (int j(0); j < inf0; ++j) {
//            arrV_sig(lo_lay - 1, i) += arrV_sig(lo_lay - 2, j) * arrV_wei(lo_lay - 2, i, j);
//        }
//    }
//
//    vector<double> out;
//    out.resize(itr_nod[lo_lay - 1]);
//    for (int i(0); i < inf; ++i) {
//        out[i] = arrV_sig(lo_lay - 1, i);
//    }
//
//    for (int j(0); j < lo_lay; ++j) {
//        for (int i(0); i < itr_nod[j]; ++i) {
//            cout << arrV_sig(j, i) << ",";
//        }
//        cout << endl;
//    }
//    cout << out[0] << endl;
//
//    return out;
//}

//vector<double> Neuralnet::ForwardG(const vector<double> &x) const
//{
//    vector<vector<double>> sig;
//    const int lo_layers(layers);
//
//    array<double, 2> *parr_sig;
//    parr_sig = new array<double, 2>(exb, Acs.get_default_view());
//    array_view<double, 2> arrV_sig = *parr_sig;
//    array_view<double, 2> arrV_bia = *parr_bia;
//    array_view<double, 3> arrV_wei = *parr_wei;
//
//    auto itr_nodes = nodes.begin();
//    auto itr_wei = wei.begin();
//    auto itr_bia = bia.begin();
//    sig.resize(lo_layers);
//    for (int i = 0; i < lo_layers; ++i) {
//        sig[i].resize(itr_nodes[i]);
//    }
//    for (int j = 0; j < itr_nodes[0]; ++j) {
//        arrV_sig(0,j) = x[j];
//    }
//    for (int i(0); i < lo_layers - 2; ++i) {
//        for (int n1(0); n1 < itr_nodes[i + 1]; ++n1) {
//            arrV_sig(i + 1,n1) = arrV_bia(i,n1);
//        }
//        for (int n0(0); n0 < itr_nodes[i]; ++n0) {
//            double lo_sig = arrV_sig(i,n0);
//            if (lo_sig != 0) {
//                for (int n1(0); n1 < itr_nodes[i + 1]; ++n1) {
//                    arrV_sig(i + 1,n1) += (lo_sig * arrV_wei(i,n1,n0));
//                }
//            }
//        }
//        for (int n1(0); n1 < itr_nodes[i + 1]; ++n1) {
//            arrV_sig(i + 1,n1) = max(0.0, arrV_sig(i + 1,n1));
//        }
//    }
//
//    int i(lo_layers - 2);
//    for (int n1(0); n1 < itr_nodes[i + 1]; ++n1) {
//        arrV_sig(i + 1,n1) = arrV_bia(i,n1);
//    }
//    for (int n0(0); n0 < itr_nodes[i]; ++n0) {
//        double lo_sig = arrV_sig(i,n0);
//        if (lo_sig != 0) {
//            for (int n1(0); n1 < itr_nodes[i + 1]; ++n1) {
//                sig[i + 1][n1] += (lo_sig * arrV_wei(i,n1,n0));
//            }
//        }
//    }
//    return sig[lo_layers - 1];
//}

//void Neuralnet::BackwardG(array_view<double, 2> &arrV_SdEdb, array_view<double, 3> &arrV_SdEdw, const vector<double> &x, const vector<double> &y) const
//{
//    array<double, 2> *parr_del;
//    array<double, 2> *parr_dEdb;
//    array<double, 3> *parr_dEdw;
//    parr_del = new array<double, 2>(exb, Acs.get_default_view());
//    parr_dEdb = new array<double, 2>(exb, Acs.get_default_view());
//    parr_dEdw = new array<double, 3>(exw, Acs.get_default_view());
//    array_view<double, 2> arrV_del = *parr_del;
//    array_view<double, 2> arrV_dEdb = *parr_dEdb;
//    array_view<double, 3> arrV_dEdw = *parr_dEdw;
//
//    //ȁE�
//    const int lo_lay(lay);
//    auto itr_nod = nod.begin();
//    array<double, 2> *parr_sig;
//    parr_sig = new array<double, 2>(exb, Acs.get_default_view());
//    array_view<double, 2> arrV_sig = *parr_sig;
//    array_view<double, 2> arrV_bia = *parr_bia;
//    array_view<double, 3> arrV_wei = *parr_wei;
//    for (int n = 0; n < itr_nod[0]; ++n) {
//        arrV_sig(0, n) = x[n];
//    }
//    for (int i(0); i < lo_lay - 2; ++i) {
//        int in0(itr_nod[i]);
//        int in1(itr_nod[i + 1]);
//        extent<1> ex(in1);
//        parallel_for_each(Acs.get_default_view(), ex, [=](index<1> idx) restrict(amp) {
//            arrV_sig(i + 1, idx[0]) = arrV_bia(i, idx[0]);
//            for (int j(0); j < in0; ++j) {
//                arrV_sig(i + 1, idx[0]) += arrV_sig(i, j) * arrV_wei(i, idx[0], j);
//            }
//            arrV_sig(i + 1, idx[0]) = max(0.0, arrV_sig(i + 1, idx[0]));
//        });
//        arrV_sig.synchronize();
//    }
//    int inf(itr_nod[lo_lay - 1]);
//    int inf0(itr_nod[lo_lay - 2]);
//    extent<1> exf(inf);
//    parallel_for_each(Acs.get_default_view(), exf, [=](index<1> idx) restrict(amp) {
//        arrV_sig(lo_lay - 1, idx[0]) = arrV_bia(lo_lay - 2, idx[0]);
//        for (int j(0); j < inf0; ++j) {
//            arrV_sig(lo_lay - 1, idx[0]) += arrV_sig(lo_lay - 2, j) * arrV_wei(lo_lay - 2, idx[0], j);
//        }
//    });
//    arrV_sig.synchronize();
//
//    //�`���恻��
//    array<double, 1> *parr_y;
//    parr_y = new array<double, 1>(exf, Acs.get_default_view());
//    array_view<double, 1> arrV_y = *parr_y;
//    for (int i(0); i < inf; ++i) {
//        arrV_y[i] = y[i];
//    }
//    parallel_for_each(Acs.get_default_view(), exf, [=](index<1> idx) restrict(amp) {
//        arrV_del(lo_lay - 2, idx[0]) = arrV_sig(lo_lay - 1, idx[0]) - arrV_y[idx[0]];
//        arrV_dEdb(lo_lay - 2, idx[0]) = arrV_del(lo_lay - 2, idx[0]);
//        arrV_SdEdb(lo_lay - 2, idx[0]) += arrV_dEdb(lo_lay - 2, idx[0]);
//        for (int n0 = 0; n0 < inf0; ++n0) {
//            arrV_dEdw(lo_lay - 2, idx[0], n0) = arrV_del(lo_lay - 2, idx[0]) * arrV_sig(lo_lay - 2, n0);
//            arrV_SdEdw(lo_lay - 2, idx[0], n0) += arrV_dEdw(lo_lay - 2, idx[0], n0);
//        }
//    });
//
//
//    for (int i = lo_lay - 3; i >= 0; --i) {
//        extent<1> ex(itr_nod[i + 1]);
//        int in2(itr_nod[i + 2]);
//        int in0(itr_nod[i]);
//        parallel_for_each(Acs.get_default_view(), ex, [=](index<1> idx) restrict(amp) {
//            arrV_del(i, idx[0]) = 0;
//            if (arrV_sig(i + 1, idx[0]) != 0) {
//                for (int n2 = 0; n2 < in2; ++n2) {
//                    arrV_del(i, idx[0]) += arrV_del(i + 1, n2) * arrV_wei(i + 1, n2, idx[0]);
//                }
//                arrV_dEdb(i, idx[0]) = arrV_del(i, idx[0]);
//                arrV_SdEdb(i, idx[0]) += arrV_dEdb(i, idx[0]);
//                for (int n0 = 0; n0 < in0; ++n0) {
//                    arrV_dEdw(i, idx[0], n0) = arrV_del(i, idx[0]) * arrV_sig(i, n0);
//                    arrV_SdEdw(i, idx[0], n0) += arrV_dEdw(i, idx[0], n0);
//                }
//            }
//        });
//    }
//}
//
//void Neuralnet::loadG(string filename)
//{
//    std::ifstream ifs(filename);
//    if (!ifs) {
//        return;
//    }
//    std::vector<int> hp(0, 0);
//    std::string str;
//    getline(ifs, str);
//    std::string token;
//    std::istringstream stream(str);
//    while (getline(stream, token, ',')) {
//        hp.emplace_back(std::stoi(token));
//    }
//
//    Set_networkG(hp);
//    array_view<double, 2> arrV_bia = *parr_bia;
//    array_view<double, 3> arrV_wei = *parr_wei;
//
//    int i(0);
//    int n1(0);
//    while (i < lay - 1) {
//        getline(ifs, str);
//        n1 = 0;
//        std::istringstream stream(str);
//        while (getline(stream, token, ',')) {
//            arrV_bia(i, n1) = std::stod(token);
//            ++n1;
//        }
//        ++i;
//    }
//
//    i = 0;
//    int n0(0);
//    while (i < lay - 1) {
//        n1 = 0;
//        while (n1 < nod[i + 1]) {
//            getline(ifs, str);
//            n0 = 0;
//            std::istringstream stream(str);
//            while (getline(stream, token, ',')) {
//                arrV_wei(i, n1, n0) = std::stod(token);
//                ++n0;
//            }
//            ++n1;
//        }
//        ++i;
//    }
//
//    ifs.close();
//}
//
//void Neuralnet::saveG(string filename) const
//{
//    std::ofstream writing_file;
//    writing_file.open(filename, std::ios::out);
//    for (int i = 0; i < lay; ++i) {
//        writing_file << nod[i] << ",";
//    }
//    writing_file << std::endl;
//    array_view<double, 2> arrV_bia = *parr_bia;
//    array_view<double, 3> arrV_wei = *parr_wei;
//
//    for (int i(0); i < lay - 1; ++i) {
//        for (int n1(0); n1 < nod[i + 1]; ++n1) {
//            writing_file << arrV_bia(i, n1) << ",";
//        }
//        writing_file << std::endl;
//    }
//    for (int i(0); i < lay - 1; ++i) {
//        for (int n1(0); n1 < nod[i + 1]; ++n1) {
//            for (int n0 = 0; n0 < nod[i]; ++n0) {
//                writing_file << arrV_wei(i, n1, n0) << ",";
//            }
//            writing_file << std::endl;
//        }
//    }
//    writing_file.close();
//}


Optimizer::Optimizer()
:a(0.001)
{
}

Optimizer::Optimizer(double a1)
: a(a1)
{
}

Optimizer::~Optimizer()
{
}

void Optimizer::operator()(Neuralnet &net, Learning &le)
{
    // for (int i = 0; i < (net.layers - 1); ++i) {
    //     for (int n1 = 0; n1 < net.nodes[i + 1]; ++n1) {
    //         net.bia[i][n1] -= a * le.SdEdb[i][n1] / le.batch;
    //         for (int n0 = 0; n0 < net.nodes[i]; ++n0) {
    //             net.wei[i][n1][n0] -= a * le.SdEdw[i][n1][n0] / le.batch;
    //         }
    //     }
    // }
	for (int i = 0; i < (net.layers - 1); ++i) {
        for (int n1 = 0; n1 < net.nodes[i + 1]; ++n1) {
            net.bia[i][n1] -= a * le.SdEdb[i][n1] / le.batch;
            for (int n0 = 0; n0 < net.nodes[i]; ++n0) {
                net.wei[i][n1][n0] -= a * le.SdEdw[i][n1][n0] / le.batch;
            }
        }
    }
    le.clearS();
}

Adam::Adam()
://a(0.001), //default 0.001
b1(0.9),   //default 0.9
b2(0.999), //default 0.999
e(1E-8)       //default 1E-8
{
}

Adam::~Adam()
{
}

void Adam::operator()(Neuralnet &net, Learning_Adam &le)
{
    double mp;
    double vp;
    for (int i = 0; i < (net.layers - 1); ++i) {
        for (int n1 = 0; n1 < net.nodes[i + 1]; ++n1) {
            for (int n0 = 1; n0 < net.nodes[i]; ++n0) {
                le.mAdamw[i][n1][n0] = le.mAdamw[i][n1][n0] * b1 + (1 - b1) * le.SdEdw[i][n1][n0] / le.batch;
                le.vAdamw[i][n1][n0] = le.vAdamw[i][n1][n0] * b2 + (1 - b2) * pow(le.SdEdw[i][n1][n0] / le.batch, 2);
                mp = le.mAdamw[i][n1][n0] / (1 - pow(b1, le.tA));
                vp = le.vAdamw[i][n1][n0] / (1 - pow(b2, le.tA));
                net.wei[i][n1][n0] -= a * mp / (sqrt(vp) + e);
            }
            le.mAdamb[i][n1] = le.mAdamb[i][n1] * b1 + (1 - b1) * le.SdEdb[i][n1] / le.batch;
            le.vAdamb[i][n1] = le.vAdamb[i][n1] * b2 + (1 - b2) * pow(le.SdEdb[i][n1] / le.batch, 2);
            mp = le.mAdamb[i][n1] / (1 - pow(b1, le.tA));
            vp = le.vAdamb[i][n1] / (1 - pow(b2, le.tA));
            net.bia[i][n1] -= a * mp / (sqrt(vp) + e);
        }
    }
    le.clearS();
    ++le.tA;
}

SDG::SDG()
{
}

SDG::SDG(double a1) : Optimizer(a1)
{
}

SDG::~SDG()
{
}

void SDG::operator()(Neuralnet &net, Learning &le)
{
    for (int i = 0; i < (net.layers - 1); ++i) {
        for (int n1 = 0; n1 < net.nodes[i + 1]; ++n1) {
            net.bia[i][n1] -= a * le.SdEdb[i][n1] / le.batch;
            for (int n0 = 1; n0 < net.nodes[i]; ++n0) {
                net.wei[i][n1][n0] -= a * le.SdEdw[i][n1][n0] / le.batch;
            }
        }
    }
    le.clearS();
}

Learning::Learning()
:learns(1000),
n_epoch(10),
batch(128),
test_ratio(0.1),
t(0),
a(0.001),
bottom_disks(56),
top_disks(58),
random(10),
show_time(0),
cut_ratio(0)
{
}

Learning::~Learning()
{
}

void Learning::read_teach_data(const string file)
{
    //�t�@�C���̓ǂݍ���
    std::ifstream ifs(file);
    if (!ifs) {
        std::cout << "CANNOT LOAD DATA\n";
        return;
    }
    teach_data_all.clear();
    teach_data_all.reserve(10000);
    //csv�t�@�C����1�s���ǂݍ���
    std::string str;
    while (getline(ifs, str)) {
        std::string token;
        std::istringstream stream(str);
        std::vector<double> tempv(0, 0);
        tempv.reserve(20);
        //1�s�̂����A������ƃR���}�𕪊�����
        while (getline(stream, token, ',')) {
            //���ׂĕ�����Ƃ��ēǂݍ��܂�邽��
            //���l�͕ϊ����K�v
            tempv.emplace_back(std::stod(token));
        }
        teach_data_all.emplace_back(tempv);
    }
    teach_data_all.shrink_to_fit();
    ifs.close();
}

void Learning::read_test_data(const string file)
{
    //�t�@�C���̓ǂݍ���
    std::ifstream ifs(file);
    if (!ifs) {
        std::cout << "CANNOT LOAD DATA\n";
        return;
    }
    test_data_all.clear();
    test_data_all.reserve(10000);
    //csv�t�@�C����1�s���ǂݍ���
    std::string str;
    while (getline(ifs, str)) {
        std::string token;
        std::istringstream stream(str);
        std::vector<double> tempv(0, 0);
        tempv.reserve(20);
        //1�s�̂����A������ƃR���}�𕪊�����
        while (getline(stream, token, ',')) {
            //���ׂĕ�����Ƃ��ēǂݍ��܂�邽��
            //���l�͕ϊ����K�v
            tempv.emplace_back(std::stod(token));
        }
        test_data_all.emplace_back(tempv);
    }
    test_data_all.shrink_to_fit();
    ifs.close();
}

void Learning::separate_data()
{
    teach_data_x.reserve(10000);
    teach_data_y.reserve(10000);
    int end(teach_data_all.size() - 1);
    for (int j = 0; j <= end; ++j) {
        vector<double> tempv(0, 0);
        for (int i = x_begin; i <= x_end; ++i) {
            tempv.emplace_back(teach_data_all[j][i]);
        }
        teach_data_x.emplace_back(tempv);
        tempv.clear();
        for (int i = y_begin; i <= y_end; ++i) {
            tempv.emplace_back(teach_data_all[j][i]);
        }
        teach_data_y.emplace_back(tempv);
    }
    teach_data_x.shrink_to_fit();
    teach_data_y.shrink_to_fit();
    
    test_data_x.reserve(10000);
    test_data_y.reserve(10000);
    end = test_data_all.size() - 1;
    for (int j = 0; j <= end; ++j) {
        std::vector<double> tempv(0, 0);
        for (int i = x_begin; i <= x_end; ++i) {
            tempv.emplace_back(test_data_all[j][i]);
        }
        test_data_x.emplace_back(tempv);
        tempv.clear();
        for (int i = y_begin; i <= y_end; ++i) {
            tempv.emplace_back(test_data_all[j][i]);
        }
        test_data_y.emplace_back(tempv);
    }
    test_data_x.shrink_to_fit();
    test_data_y.shrink_to_fit();
}

void Learning::set_paras(const vector<double> &x, const vector<int> &n)
{
	n_epoch = int(x[0]);
    batch = int(x[2]);
    test_ratio = int(x[3]);
    t = int(x[4]);
    net = n;
    x_begin = n[0];
    x_end = n[n.size() - 1];
    y_begin = 0;
    y_end = 0;
    ai_accuracy.resize(0);
    ai_accuracy_sign.resize(0);
    teach_data_x.resize(0);
    teach_data_y.resize(0);
    test_data_x.resize(0);
    SdEdb.resize(net.size() - 1);
    SdEdw.resize(net.size() - 1);
    for (unsigned int i = 0; i < net.size() - 1; ++i) {
        SdEdb[i].resize(net[i + 1], 0);
        SdEdw[i].resize(net[i + 1]);
        for (int j = 0; j < net[i + 1]; ++j) {
            SdEdw[i][j].assign(net[i], 0);
        }
    }
}

// void Learning_Adam::set_parameters(const vector<double> &x, const vector<int> &n)
// {
//     times = int(x[0]);
//     batch = int(x[1]);
//     x_begin = int(x[2]);
//     x_end = int(x[3]);
//     y_begin = int(x[4]);
//     y_end = int(x[5]);
//     tests = int(x[6]);
//     t = int(x[7]);
//     a = x[8];
//     tA = int(x[9]);
//     net = n;
//     ai_accuracy.reserve(times*batch + 1);
//     SdEdb.resize(net.size() - 1);
//     SdEdw.resize(net.size() - 1);
//     mAdamb.resize(net.size() - 1);
//     vAdamb.resize(net.size() - 1);
//     mAdamw.resize(net.size() - 1);
//     vAdamw.resize(net.size() - 1);
//     for (unsigned int i = 0; i < net.size() - 1; ++i) {
//         SdEdb[i].resize(net[i + 1]);
//         SdEdw[i].resize(net[i + 1]);
//         mAdamb[i].resize(net[i + 1]);
//         vAdamb[i].resize(net[i + 1]);
//         mAdamw[i].resize(net[i + 1]);
//         vAdamw[i].resize(net[i + 1]);
//         for (int j = 0; j < net[i + 1]; ++j) {
//             SdEdw[i][j].assign(net[i], 0);
//             mAdamw[i][j].assign(net[i], 0);
//             vAdamw[i][j].assign(net[i], 0);
//         }
//     }
// }

void Learning::clearS()
{
    for (unsigned int i = 0; i < net.size() - 1; ++i) {
        for (int n1 = 0; n1 < net[i + 1]; ++n1) {
            SdEdb[i][n1] = 0;
            for (int n0 = 0; n0 < net[i]; ++n0) {
                SdEdw[i][n1][n0] = 0;
            }
        }
    }
}

void Learning::save_log(string filename, double train_loss, double test_loss, double msec) const
{
    std::ofstream writing_file;
    writing_file.open(filename, std::ios::app);

    writing_file << games << "," << bottom_disks << "," << top_disks << ","
	<< playing_search_depth << "," << teaching_search_depth << "," << a << ","
	<< random << "," << train_loss << "," << test_loss << "," << msec << endl;
    writing_file.close();
}

void Learning_Adam::save(string filename) const
{
    std::ofstream writing_file;
    writing_file.open(filename, std::ios::out);
    
    writing_file << "," << batch << "," << x_begin << ","
    << x_end << "," << y_begin << "," << y_end << "," << ","
    << t << "," << a << "," << tA << "," << std::endl;
    for (unsigned int i = 0; i < net.size(); ++i) {
        writing_file << net[i] << ",";
    }
    writing_file << std::endl;
    for (unsigned int i(0); i < net.size() - 1; ++i) {
        for (int n1(0); n1 < net[i + 1]; ++n1) {
            writing_file << mAdamb[i][n1] << ",";
        }
        writing_file << std::endl;
    }
    for (unsigned int i(0); i < net.size() - 1; ++i) {
        for (int n1(0); n1 < net[i + 1]; ++n1) {
            writing_file << vAdamb[i][n1] << ",";
        }
        writing_file << std::endl;
    }
    for (unsigned int i(0); i < net.size() - 1; ++i) {
        for (int n1(0); n1 < net[i + 1]; ++n1) {
            for (int n0 = 0; n0 < net[i]; ++n0) {
                writing_file << mAdamw[i][n1][n0] << ",";
            }
            writing_file << std::endl;
        }
    }
    for (unsigned int i(0); i < net.size() - 1; ++i) {
        for (int n1(0); n1 < net[i + 1]; ++n1) {
            for (int n0 = 0; n0 < net[i]; ++n0) {
                writing_file << vAdamw[i][n1][n0] << ",";
            }
            writing_file << std::endl;
        }
    }
    writing_file << t << "," << std::endl;
    writing_file << tA << "," << std::endl;
    writing_file.close();
}

void Learning::load(string filename)
{
	map<string, string> m(map_read_file(filename));
	learns = stoi(m["learns"]);
	n_epoch = stoi(m["n_epoch"]);
    games = stoi(m["games"]);
    batch = stoi(m["batch"]);
    t = stoi(m["t"]);
    a = stod(m["a"]);
	bottom_disks = stoi(m["bottom_disks"]);
	top_disks = stoi(m["top_disks"]);
	playing_search_depth = stoi(m["playing_search_depth"]);
	teaching_search_depth = stoi(m["teaching_search_depth"]);
	perfect_search_depth = stoi(m["perfect_search_depth"]);
	random = stod(m["random"]);
	show_time = stoi(m["show_time"]);
	cut_ratio = stod(m["cut_ratio"]);

	string n = m["net"];
	net = vector<int>(0);
    istringstream stream(n);
	string token;
    while (getline(stream, token, ',')) {
        net.emplace_back(stoi(token));
	}

    SdEdb.resize(net.size() - 1);
    SdEdw.resize(net.size() - 1);
    for (unsigned int i = 0; i < net.size() - 1; ++i) {
        SdEdb[i].resize(net[i + 1], 0);
        SdEdw[i].resize(net[i + 1]);
        for (int j = 0; j < net[i + 1]; ++j) {
            SdEdw[i][j].assign(net[i], 0);
        }
    }
}

void Learning_Adam::load(string filename)
{
    std::ifstream ifs(filename);
    if (!ifs) {
        std::cout << "CANNOT LOAD DATA\n";
        return;
    }
    std::vector<double> hp(0, 0);
    std::string str;
    getline(ifs, str);
    std::string token;
    std::istringstream stream(str);
    while (getline(stream, token, ',')) {
        hp.emplace_back(std::stoi(token));
    }
    getline(ifs, str);
    while (getline(stream, token, ',')) {
        net.emplace_back(std::stoi(token));
    }
    
    set_paras(hp, net);
    
    unsigned int i(0);
    int n1(0);
    int n0(0);
    while (i < net.size() - 1) {
        getline(ifs, str);
        std::istringstream stream(str);
        n1 = 0;
        while (getline(stream, token, ',')) {
            mAdamb[i][n1] = std::stod(token);
            ++n1;
        }
        ++i;
    }
    
    i = 0;
    while (i < net.size() - 1) {
        getline(ifs, str);
        std::istringstream stream(str);
        n1 = 0;
        while (getline(stream, token, ',')) {
            vAdamb[i][n1] = std::stod(token);
            ++n1;
        }
        ++i;
    }
    
    i = 0;
    while (i < net.size() - 1) {
        n1 = 0;
        while (n1 < net[i + 1]) {
            getline(ifs, str);
            n0 = 0;
            std::istringstream stream(str);
            while (getline(stream, token, ',')) {
                mAdamw[i][n1][n0] = std::stod(token);
                ++n0;
            }
            ++n1;
        }
        ++i;
    }
    
    i = 0;
    while (i < net.size() - 1) {
        n1 = 0;
        while (n1 < net[i + 1]) {
            getline(ifs, str);
            n0 = 0;
            std::istringstream stream(str);
            while (getline(stream, token, ',')) {
                vAdamw[i][n1][n0] = std::stod(token);
                ++n0;
            }
            ++n1;
        }
        ++i;
    }
    getline(ifs, str);
    std::istringstream stream2(str);
    while (getline(stream2, token, ',')) {
        t = std::stoi(token);
    }
    while (getline(stream2, token, ',')) {
        tA = std::stoi(token);
    }
    
    ifs.close();
}

Learning_Adam::Learning_Adam()
:tA(0)
{
}

Learning_Adam::~Learning_Adam()
{
}
