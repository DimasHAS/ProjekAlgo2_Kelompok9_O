/**
 * SISTEM MANAJEMEN KEUANGAN PRIBADI (C++)
 * 
 * Program ini adalah aplikasi manajemen keuangan modular yang menggunakan:
 * 1. Struct untuk data transaksi
 * 2. Array of Struct untuk penyimpanan sementara
 * 3. File Handling (fstream) untuk persistensi data
 * 4. Bubble Sort untuk pengurutan data
 * 5. Linear Search untuk pencarian data
 * 6. Fungsi Rekursif untuk menghitung total saldo
 */

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>

using namespace std;

// 1. Struktur Data untuk Transaksi
struct Transaksi {
    int id;
    string tanggal;
    string kategori;  // "Pemasukan" atau "Pengeluaran"
    string deskripsi;
    double jumlah;
};

// Global variables (atau bisa dipassing dalam fungsi)
const int MAX_TRANSAKSI = 1000;
Transaksi daftarTransaksi[MAX_TRANSAKSI];
int jumlahData = 0;
const string FILENAME = "data_keuangan.txt";

// Prototipe Fungsi
void tambahTransaksi();
void lihatRiwayat();
void urutkanData();
void cariTransaksi();
void laporanKeuangan();
void simpanKeFile();
void bacaDariFile();
double hitungTotalRekursif(int n);
void tampilkanHeader();

// --- FUNGSI UTAMA ---
int main() {
    bacaDariFile();
    int pilihan;

    do {
        system("clear || cls"); // Membersihkan layar
        cout << "==========================================" << endl;
        cout << "   PERSONAL FINANCE MANAGER (C++)         " << endl;
        cout << "==========================================" << endl;
        cout << "1. Tambah Transaksi" << endl;
        cout << "2. Lihat Riwayat (Tabel)" << endl;
        cout << "3. Urutkan Data (Nominal)" << endl;
        cout << "4. Cari Transaksi (Deskripsi)" << endl;
        cout << "5. Laporan Keuangan (Rekursif)" << endl;
        cout << "6. Simpan & Keluar" << endl;
        cout << "------------------------------------------" << endl;
        cout << "Pilih Menu (1-6): ";
        cin >> pilihan;
        cin.ignore(); // Membersihkan buffer

        switch (pilihan) {
            case 1: tambahTransaksi(); break;
            case 2: lihatRiwayat(); break;
            case 3: urutkanData(); break;
            case 4: cariTransaksi(); break;
            case 5: laporanKeuangan(); break;
            case 6: simpanKeFile(); cout << "Data tersimpan. Terima kasih!" << endl; break;
            default: cout << "Pilihan tidak valid!" << endl;
        }

        if (pilihan != 6) {
            cout << "\nTekan Enter untuk kembali ke menu...";
            cin.get();
        }
    } while (pilihan != 6);

    return 0;
}

// 2. Fungsi Tambah Transaksi
void tambahTransaksi() {
    if (jumlahData >= MAX_TRANSAKSI) {
        cout << "Error: Kapasitas penuh!" << endl;
        return;
    }

    Transaksi t;
    t.id = (jumlahData > 0) ? daftarTransaksi[jumlahData-1].id + 1 : 1;
    
    cout << "\n--- INPUT TRANSAKSI BARU ---" << endl;
    cout << "Tanggal (DD/MM/YYYY): "; getline(cin, t.tanggal);
    
    int kat;
    cout << "Kategori (1. Pemasukan, 2. Pengeluaran): "; cin >> kat;
    t.kategori = (kat == 1) ? "Pemasukan" : "Pengeluaran";
    cin.ignore();
    
    cout << "Deskripsi: "; getline(cin, t.deskripsi);
    cout << "Nominal (Jumlah): "; cin >> t.jumlah;
    cin.ignore();

    daftarTransaksi[jumlahData] = t;
    jumlahData++;
    cout << "Berhasil menambahkan data!" << endl;
}

// Fungsi Helper: Tampilan Header Tabel
void tampilkanHeader() {
    cout << left << setw(5) << "ID" 
         << setw(12) << "Tanggal" 
         << setw(15) << "Kategori" 
         << setw(20) << "Deskripsi" 
         << "Nominal" << endl;
    cout << string(60, '-') << endl;
}

// 3. Fungsi Lihat Riwayat (Tampilan Tabel)
void lihatRiwayat() {
    cout << "\n--- RIWAYAT TRANSAKSI ---" << endl;
    if (jumlahData == 0) {
        cout << "Data masih kosong." << endl;
        return;
    }
    tampilkanHeader();
    for (int i = 0; i < jumlahData; i++) {
        cout << left << setw(5) << daftarTransaksi[i].id 
             << setw(12) << daftarTransaksi[i].tanggal 
             << setw(15) << daftarTransaksi[i].kategori 
             << setw(20) << daftarTransaksi[i].deskripsi 
             << fixed << setprecision(2) << daftarTransaksi[i].jumlah << endl;
    }
}

// 4. Algoritma Sorting (Bubble Sort) berdasarkan Nominal Terbesar
void urutkanData() {
    if (jumlahData < 2) return;
    
    // Bubble Sort
    for (int i = 0; i < jumlahData - 1; i++) {
        for (int j = 0; j < jumlahData - i - 1; j++) {
            if (daftarTransaksi[j].jumlah < daftarTransaksi[j+1].jumlah) {
                // Swap struct
                Transaksi temp = daftarTransaksi[j];
                daftarTransaksi[j] = daftarTransaksi[j+1];
                daftarTransaksi[j+1] = temp;
            }
        }
    }
    cout << "Data berhasil diurutkan berdasarkan nominal terbesar!" << endl;
    lihatRiwayat();
}

// 5. Algoritma Searching (Linear Search) berdasarkan Deskripsi
void cariTransaksi() {
    string keyword;
    cout << "Masukkan kata kunci deskripsi: ";
    getline(cin, keyword);

    bool ketemu = false;
    cout << "\n--- HASIL PENCARIAN ---" << endl;
    tampilkanHeader();
    for (int i = 0; i < jumlahData; i++) {
        // Simple case-sensitive check
        if (daftarTransaksi[i].deskripsi.find(keyword) != string::npos) {
            cout << left << setw(5) << daftarTransaksi[i].id 
                 << setw(12) << daftarTransaksi[i].tanggal 
                 << setw(15) << daftarTransaksi[i].kategori 
                 << setw(20) << daftarTransaksi[i].deskripsi 
                 << fixed << setprecision(2) << daftarTransaksi[i].jumlah << endl;
            ketemu = true;
        }
    }
    if (!ketemu) cout << "Data tidak ditemukan." << endl;
}

// 6. Fungsi Rekursif untuk menghitung Saldo Akhir
// Saldo = Total Pemasukan - Total Pengeluaran
double hitungTotalRekursif(int n) {
    if (n < 0) return 0;
    
    double nilai = (daftarTransaksi[n].kategori == "Pemasukan") ? 
                    daftarTransaksi[n].jumlah : -daftarTransaksi[n].jumlah;
                    
    return nilai + hitungTotalRekursif(n - 1);
}

void laporanKeuangan() {
    cout << "\n--- LAPORAN KEUANGAN ---" << endl;
    if (jumlahData == 0) {
        cout << "Belum ada transaksi." << endl;
        return;
    }
    double totalSaldo = hitungTotalRekursif(jumlahData - 1);
    cout << "Total Transaksi: " << jumlahData << endl;
    cout << "Estimasi Saldo Akhir: Rp" << fixed << setprecision(2) << totalSaldo << endl;
    if (totalSaldo < 0) cout << "Catatan: Pengeluaran lebih besar dari pemasukan!" << endl;
}

// 7. File Handling: Simpan ke File
void simpanKeFile() {
    ofstream file(FILENAME);
    if (file.is_open()) {
        for (int i = 0; i < jumlahData; i++) {
            file << daftarTransaksi[i].id << "|"
                 << daftarTransaksi[i].tanggal << "|"
                 << daftarTransaksi[i].kategori << "|"
                 << daftarTransaksi[i].deskripsi << "|"
                 << daftarTransaksi[i].jumlah << endl;
        }
        file.close();
    }
}

// 7. File Handling: Baca dari File
void bacaDariFile() {
    ifstream file(FILENAME);
    if (file.is_open()) {
        string line;
        jumlahData = 0;
        while (getline(file, line) && jumlahData < MAX_TRANSAKSI) {
            size_t pos = 0;
            string token;
            vector<string> data;
            
            // Parsing string dengan separator '|'
            while ((pos = line.find("|")) != string::npos) {
                token = line.substr(0, pos);
                data.push_back(token);
                line.erase(0, pos + 1);
            }
            data.push_back(line); // Last token

            if (data.size() == 5) {
                daftarTransaksi[jumlahData].id = stoi(data[0]);
                daftarTransaksi[jumlahData].tanggal = data[1];
                daftarTransaksi[jumlahData].kategori = data[2];
                daftarTransaksi[jumlahData].deskripsi = data[3];
                daftarTransaksi[jumlahData].jumlah = stod(data[4]);
                jumlahData++;
            }
        }
        file.close();
    }
}
