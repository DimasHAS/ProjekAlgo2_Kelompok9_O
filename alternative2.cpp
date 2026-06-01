#include <iostream>
#include <fstream>
#include <string>
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

// Global variables - Menggunakan Raw Pointer untuk manajemen memori dinamis
const int MAX_TRANSAKSI = 1000;
Transaksi* daftarTransaksi = nullptr; // Pointer ke array transaksi dinamis
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
void alokasikanMemori();
void bebaskanMemori();

// --- FUNGSI UTAMA ---
int main() {
    // 1. Alokasikan memori dinamis
    alokasikanMemori();
    
    // 2. Baca data keuangan dari file
    bacaDariFile();
    
    int pilihan;

    do {
        // Membersihkan layar konsol secara portabel
        #ifdef _WIN32
            system("cls");
        #else
            system("clear");
        #endif

        cout << "======================================================" << endl;
        cout << "   PERSONAL FINANCE MANAGER (C++ Pointer & Array) " << endl;
        cout << "======================================================" << endl;
        cout << "1. Tambah Transaksi" << endl;
        cout << "2. Lihat Riwayat (Tabel)" << endl;
        cout << "3. Urutkan Data (Nominal Terbesar)" << endl;
        cout << "4. Cari Transaksi (Deskripsi)" << endl;
        cout << "5. Laporan Keuangan (Rekursif)" << endl;
        cout << "6. Simpan & Keluar" << endl;
        cout << "------------------------------------------------------" << endl;
        cout << "Pilih Menu (1-6): ";
        
        // Error handling apabila pengguna memasukkan input non-numerik
        if (!(cin >> pilihan)) {
            cout << "Error: Input tidak valid! Harap masukkan angka 1-6." << endl;
            cin.clear();             // Membersihkan error flag dari cin stream
            cin.ignore(10000, '\n'); // Membersihkan karakter sisa di buffer
            pilihan = 0;             // Reset pilihan agar berulang kembali
        } else {
            cin.ignore(10000, '\n'); // Bersihkan sisa karakter buffer (termasuk newline)
        }

        switch (pilihan) {
            case 1: tambahTransaksi(); break;
            case 2: lihatRiwayat(); break;
            case 3: urutkanData(); break;
            case 4: cariTransaksi(); break;
            case 5: laporanKeuangan(); break;
            case 6: 
                simpanKeFile(); 
                cout << "Seluruh data berhasil ditulis ke " << FILENAME << endl;
                cout << "Program selesai. Terima kasih!" << endl; 
                break;
            default: 
                if (pilihan != 0) {
                    cout << "Pilihan salah! Silakan masukkan opsi 1 hingga 6." << endl;
                }
        }

        if (pilihan != 6) {
            cout << "\nTekan Enter untuk kembali ke menu utama...";
            cin.get();
        }
    } while (pilihan != 6);

    // Bebaskan memori sebelum program exit
    bebaskanMemori();
    return 0;
}

// Implementasi Alokasi Memori
void alokasikanMemori() {
    daftarTransaksi = new Transaksi[MAX_TRANSAKSI];
    if (daftarTransaksi == nullptr) {
        cerr << "CRITICAL ERROR: Memori sistem penuh. Gagal mengalokasikan array!" << endl;
        exit(1);
    }
}

// Implementasi Bebaskan Memori
void bebaskanMemori() {
    if (daftarTransaksi != nullptr) {
        delete[] daftarTransaksi;
        daftarTransaksi = nullptr;
    }
}



// Fungsi Tambah Transaksi
void tambahTransaksi() {
    if (daftarTransaksi == nullptr) {
        cerr << "Error: Heap memori belum diinisialisasi!" << endl;
        return;
    }
    if (jumlahData >= MAX_TRANSAKSI) {
        cout << "Error: Batas kapasitas transaksi penuh!" << endl;
        return;
    }

    Transaksi* t = &daftarTransaksi[jumlahData];
    t->id = (jumlahData > 0) ? daftarTransaksi[jumlahData-1].id + 1 : 1;
    
    cout << "\n--- INPUT TRANSAKSI BARU ---" << endl;
    cout << "Tanggal (DD/MM/YYYY): "; 
    getline(cin, t->tanggal);
    if (t->tanggal.empty()) {
        t->tanggal = "null";
    }
    
    int kat = 0;
    while (true) {
        cout << "Kategori (1. Pemasukan, 2. Pengeluaran): "; 
        if (cin >> kat && (kat == 1 || kat == 2)) {
            cin.ignore(10000, '\n');
            break;
        } else {
            cout << "Error: Input salah! Masukkan angka 1 atau 2." << endl;
            cin.clear();
            cin.ignore(10000, '\n');
        }
    }
    t->kategori = (kat == 1) ? "Pemasukan" : "Pengeluaran";
    
    cout << "Deskripsi: "; 
    getline(cin, t->deskripsi);
    if (t->deskripsi.empty()) {
        t->deskripsi = "null";
    }
    
    // Validasi input nominal harus di atas nol
    while (true) {
        cout << "Nominal (Jumlah > 0): "; 
        if (cin >> t->jumlah && t->jumlah >= 0) {
            cin.ignore(10000, '\n');
            break;
        } else {
            cout << "Error: Nominal salah! Harap masukkan angka positif." << endl;
            cin.clear();
            cin.ignore(10000, '\n');
        }
    }

    jumlahData++;
    cout << "Data transaksi baru berhasil dimasukkan ke memori!" << endl;
}

// Cetak header tabel
void tampilkanHeader() {
    cout << left << setw(6) << "ID"
         << setw(14) << "Tanggal"
         << setw(16) << "Kategori"
         << setw(25) << "Deskripsi"
         << "Nominal (Rp)" << endl;
    cout << string(73, '-') << endl;
}

// Lihat Riwayat Transaksi
void lihatRiwayat() {
    cout << "\n--- RIWAYAT TRANSAKSI ---" << endl;
    if (daftarTransaksi == nullptr) {
        cerr << "Error: Memori tidak dapat diakses!" << endl;
        return;
    }
    if (jumlahData == 0) {
        cout << "Belum ada transaksi di dalam memori." << endl;
        return;
    }
    
    tampilkanHeader();
    for (int i = 0; i < jumlahData; i++) {
        Transaksi* t = &daftarTransaksi[i];
        cout << left << setw(6) << t->id
             << setw(14) << t->tanggal
             << setw(16) << t->kategori
             << setw(25) << t->deskripsi
             << "Rp " << fixed << setprecision(2) << t->jumlah << endl;
    }
}

// Urutkan Data Transaksi (Bubble Sort berkas pointer)
void urutkanData() {
    if (daftarTransaksi == nullptr) return;
    if (jumlahData < 2) {
        cout << "Data terlalu sedikit (butuh minimal 2 data untuk menyortir)." << endl;
        return;
    }
    
    // Bubble Sort dengan pertukaran swap pointer objek
    for (int i = 0; i < jumlahData - 1; i++) {
        for (int j = 0; j < jumlahData - i - 1; j++) {
            if (daftarTransaksi[j].jumlah < daftarTransaksi[j+1].jumlah) {
                // Point ke record yang akan ditukar
                Transaksi* t1 = &daftarTransaksi[j];
                Transaksi* t2 = &daftarTransaksi[j+1];
                
                // Swap struct data secara pointer-to-object dereference
                Transaksi temp = *t1;
                *t1 = *t2;
                *t2 = temp;
            }
        }
    }
    cout << "Sukses: Data berhasil disortir berdasarkan nominal terbesar!" << endl;
    lihatRiwayat();
}

// Linear Search dengan pointer
void cariTransaksi() {
    if (daftarTransaksi == nullptr) return;
    string keyword;
    cout << "Masukkan kata kunci deskripsi: ";
    getline(cin, keyword);

    bool ketemu = false;
    cout << "\n--- HASIL PENCARIAN ---" << endl;
    tampilkanHeader();
    for (int i = 0; i < jumlahData; i++) {
        Transaksi* t = &daftarTransaksi[i];
        if (t->deskripsi.find(keyword) != string::npos) {
            cout << left << setw(6) << t->id
                 << setw(14) << t->tanggal
                 << setw(16) << t->kategori
                 << setw(25) << t->deskripsi
                 << "Rp " << fixed << setprecision(2) << t->jumlah << endl;
            ketemu = true;
        }
    }
    if (!ketemu) cout << "Pencarian nihil! Tidak ada transaksi dengan kata kunci: " << keyword << endl;
}

// Fungsi Rekursif Pointer-based untuk hitung saldo akhir
double hitungTotalRekursif(int n) {
    if (n < 0 || daftarTransaksi == nullptr) return 0;
    
    Transaksi* t = &daftarTransaksi[n];
    double nilaiVal = (t->kategori == "Pemasukan") ? t->jumlah : -t->jumlah;
                    
    return nilaiVal + hitungTotalRekursif(n - 1);
}

// Tampilkan Laporan Keuangan
void laporanKeuangan() {
    cout << "\n--- LAPORAN KEUANGAN ---" << endl;
    if (jumlahData == 0) {
        cout << "Belum ada catatan keuangan saat ini." << endl;
        return;
    }
    double totalSaldo = hitungTotalRekursif(jumlahData - 1);
    cout << "Total Item Transaksi: " << jumlahData << endl;
    cout << "Estimasi Saldo Akhir: Rp " << fixed << setprecision(2) << totalSaldo << endl;
    if (totalSaldo < 0) {
        cout << "Catatan: Pengeluaran melebihi pemasukan (Defisit keuangan)!" << endl;
    } else {
        cout << "Status Keuangan     : Surplus (Sehat)" << endl;
    }
}

// Simpan data dari dynamic array ke file
void simpanKeFile() {
    if (daftarTransaksi == nullptr) return;
    ofstream file(FILENAME);
    if (!file) {
        cerr << "Error: Gagal membuka berkas data " << FILENAME << " untuk menyimpan!" << endl;
        return;
    }
    
    for (int i = 0; i < jumlahData; i++) {
        Transaksi* t = &daftarTransaksi[i];
        file << t->id << "|"
             << t->tanggal << "|"
             << t->kategori << "|"
             << t->deskripsi << "|"
             << t->jumlah << endl;
    }
    file.close();
    cout << "Data tersimpan permanen ke file: " << FILENAME << endl;
}

// Baca data dari file dengan array standar parsing (atoi, atof) tanpa exception
void bacaDariFile() {
    ifstream file(FILENAME);
    if (!file) {
        cout << "Info: File '" << FILENAME << "' tidak ditemukan. Memulai dari database baru." << endl;
        return;
    }
    
    string line;
    jumlahData = 0;
    
    while (getline(file, line) && jumlahData < MAX_TRANSAKSI) {
        // Menggunakan array string biasa berukuran konstan, bukan vector
        string data[5];
        int tokenCount = 0;
        size_t pos = 0;
        string tempLine = line;
        
        while (tokenCount < 4 && (pos = tempLine.find("|")) != string::npos) {
            data[tokenCount++] = tempLine.substr(0, pos);
            tempLine.erase(0, pos + 1);
        }
        if (tokenCount < 5) {
            data[tokenCount++] = tempLine;
        }

        if (tokenCount == 5) {
            Transaksi* t = &daftarTransaksi[jumlahData];
            
            // Konversi string ke angka menggunakan fungsi pustaka standar cstdlib (atoi, atof)
            t->id = atoi(data[0].c_str());
            t->tanggal = data[1];
            t->kategori = data[2];
            t->deskripsi = data[3];
            t->jumlah = atof(data[4].c_str());
            
            // Validasi data untuk membuang entri rusak di luar try-catch
            if (t->jumlah >= 0 && (t->kategori == "Pemasukan" || t->kategori == "Pengeluaran")) {
                jumlahData++;
            } else {
                cerr << "Warning: Format baris " << (jumlahData + 1) << " tidak valid! Dilewati." << endl;
            }
        }
    }
    file.close();
    cout << "Sukses: Memuat " << jumlahData << " transaksi aktif dari berkas " << FILENAME << endl;
}
