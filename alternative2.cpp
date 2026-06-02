#include <iostream>
#include <fstream>
#include <string>
using namespace std;

struct Transaksi {
    int id;
    string tanggal;
    string kategori;
    string deskripsi;
    double jumlah;
};

const int MAX_TRANSAKSI = 1000;
Transaksi* daftarTransaksi = nullptr;
int jumlahData = 0;
const string FILENAME = "data_keuangan.txt";

void tambahTransaksi();
void lihatRiwayat();
void urutkanData();
void cariTransaksi();
void laporanKeuangan();
void editTransaksi();
void hapusTransaksi();
void simpanKeFile();
void bacaDariFile();
double hitungTotalRekursif(int n);
void tampilkanHeader();
void alokasikanMemori();
void bebaskanMemori();

int main() {
    alokasikanMemori();
    bacaDariFile();
    
    int pilihan;

    do {
        system("cls");

        cout << "======================================================" << endl;
        cout << "              PERSONAL FINANCE MANAGER " << endl;
        cout << "======================================================" << endl;
        cout << "1. Tambah Transaksi" << endl;
        cout << "2. Lihat Riwayat" << endl;
        cout << "3. Urutkan Data" << endl;
        cout << "4. Cari Transaksi" << endl;
        cout << "5. Laporan Keuangan" << endl;
        cout << "6. Edit Transaksi" << endl;
        cout << "7. Hapus Transaksi" << endl;
        cout << "8. Simpan & Keluar" << endl;
        cout << "------------------------------------------------------" << endl;
        cout << "Pilih Menu (1-8): ";
        
        if (!(cin >> pilihan)) {
            cout << "Error: Input tidak valid! Harap masukkan angka 1-8." << endl;
            cin.clear();             
            cin.ignore(10000, '\n'); 
            pilihan = 0;             
        } else {
            cin.ignore(10000, '\n'); 
        }

        switch (pilihan) {
            case 1: tambahTransaksi(); break;
            case 2: lihatRiwayat(); break;
            case 3: urutkanData(); break;
            case 4: cariTransaksi(); break;
            case 5: laporanKeuangan(); break;
            case 6: editTransaksi(); break;
            case 7: hapusTransaksi(); break;
            case 8: 
                simpanKeFile(); 
                cout << "Seluruh data berhasil ditulis ke " << FILENAME << endl;
                cout << "Program selesai. Terima kasih!" << endl; 
                break;
            default: 
                if (pilihan != 0) {
                    cout << "Pilihan salah! Silakan masukkan opsi 1 hingga 8." << endl;
                }
        }

        if (pilihan != 8) {
            cout << "\nTekan Enter untuk kembali ke menu utama...";
            cin.get();
        }
    } while (pilihan != 8);

    bebaskanMemori();
    return 0;
}

void alokasikanMemori() {
    daftarTransaksi = new Transaksi[MAX_TRANSAKSI];
    if (daftarTransaksi == nullptr) {
        cerr << "CRITICAL ERROR: Memori sistem penuh. Gagal mengalokasikan array!" << endl;
        exit(1);
    }
}

void bebaskanMemori() {
    if (daftarTransaksi != nullptr) {
        delete[] daftarTransaksi;
        daftarTransaksi = nullptr;
    }
}

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

void tampilkanHeader() {
    cout << "ID     Tanggal         Kategori        Deskripsi             Nominal (Rp)" << endl;
    cout << string(80, '-') << endl;
}

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
        string id = to_string(t->id);
        string jumlahStr = to_string(t->jumlah).substr(0, to_string(t->jumlah).find('.') + 3);
        cout << id;
        for (int j = 0; j < 6 - id.length(); j++) cout << " ";
        cout << t->tanggal;
        for (int j = 0; j < 14 - t->tanggal.length(); j++) cout << " ";
        cout << t->kategori;
        for (int j = 0; j < 16 - t->kategori.length(); j++) cout << " ";
        cout << t->deskripsi;
        for (int j = 0; j < 25 - t->deskripsi.length(); j++) cout << " ";
        cout << "Rp " << jumlahStr << endl;
    }
}

void urutkanData() {
    if (daftarTransaksi == nullptr) return;
    if (jumlahData < 2) {
        cout << "Data terlalu sedikit (butuh minimal 2 data untuk menyortir)." << endl;
        return;
    }
    
    for (int i = 0; i < jumlahData - 1; i++) {
        for (int j = 0; j < jumlahData - i - 1; j++) {
            if (daftarTransaksi[j].jumlah < daftarTransaksi[j+1].jumlah) {

                Transaksi* t1 = &daftarTransaksi[j];
                Transaksi* t2 = &daftarTransaksi[j+1];
                
                Transaksi temp = *t1;
                *t1 = *t2;
                *t2 = temp;
            }
        }
    }
    cout << "Sukses: Data berhasil disortir berdasarkan nominal terbesar!" << endl;
    lihatRiwayat();
}

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
            string id = to_string(t->id);
            string jumlahStr = to_string(t->jumlah).substr(0, to_string(t->jumlah).find('.') + 3);
            cout << id;
            for (int j = 0; j < 6 - id.length(); j++) cout << " ";
            cout << t->tanggal;
            for (int j = 0; j < 14 - t->tanggal.length(); j++) cout << " ";
            cout << t->kategori;
            for (int j = 0; j < 16 - t->kategori.length(); j++) cout << " ";
            cout << t->deskripsi;
            for (int j = 0; j < 25 - t->deskripsi.length(); j++) cout << " ";
            cout << "Rp " << jumlahStr << endl;
            ketemu = true;
        }
    }
    if (!ketemu) cout << "Pencarian nihil! Tidak ada transaksi dengan kata kunci: " << keyword << endl;
}

double hitungTotalRekursif(int n) {
    if (n < 0 || daftarTransaksi == nullptr) return 0;
    
    Transaksi* t = &daftarTransaksi[n];
    double nilaiVal = (t->kategori == "Pemasukan") ? t->jumlah : -t->jumlah;
                    
    return nilaiVal + hitungTotalRekursif(n - 1);
}

void laporanKeuangan() {
    cout << "\n--- LAPORAN KEUANGAN ---" << endl;
    if (jumlahData == 0) {
        cout << "Belum ada catatan keuangan saat ini." << endl;
        return;
    }
    double totalSaldo = hitungTotalRekursif(jumlahData - 1);
    cout << "Total Item Transaksi: " << jumlahData << endl;
    string totalStr = to_string(totalSaldo);
    string totalFormatted = totalStr.substr(0, totalStr.find('.') + 3);
    cout << "Estimasi Saldo Akhir: Rp " << totalFormatted << endl;
    if (totalSaldo < 0) {
        cout << "Catatan: Pengeluaran melebihi pemasukan (Defisit keuangan)!" << endl;
    } else {
        cout << "Status Keuangan     : Surplus (Sehat)" << endl;
    }
}

void editTransaksi() {
    if (daftarTransaksi == nullptr) return;
    if (jumlahData == 0) {
        cout << "Belum ada transaksi untuk diedit." << endl;
        return;
    }
    
    lihatRiwayat();
    
    int idEdit;
    cout << "\n--- EDIT TRANSAKSI ---" << endl;
    cout << "Masukkan ID transaksi yang ingin diedit: ";
    cin >> idEdit;
    cin.ignore(10000, '\n');
    
    int indexEdit = -1;
    for (int i = 0; i < jumlahData; i++) {
        if (daftarTransaksi[i].id == idEdit) {
            indexEdit = i;
            break;
        }
    }
    
    if (indexEdit == -1) {
        cout << "Error: ID transaksi tidak ditemukan!" << endl;
        return;
    }
    
    Transaksi* t = &daftarTransaksi[indexEdit];
    
    cout << "\n--- EDIT DATA TRANSAKSI ID " << t->id << " ---" << endl;
    cout << "Tanggal saat ini: " << t->tanggal << endl;
    cout << "Tanggal baru (DD/MM/YYYY) [Enter jika tidak diubah]: ";
    string tanggalBaru;
    getline(cin, tanggalBaru);
    if (!tanggalBaru.empty()) {
        t->tanggal = tanggalBaru;
    }
    
    cout << "Kategori saat ini: " << t->kategori << endl;
    int katBaru = 0;
    cout << "Kategori baru (1. Pemasukan, 2. Pengeluaran) [0 jika tidak diubah]: ";
    cin >> katBaru;
    cin.ignore(10000, '\n');
    if (katBaru == 1 || katBaru == 2) {
        t->kategori = (katBaru == 1) ? "Pemasukan" : "Pengeluaran";
    }
    
    cout << "Deskripsi saat ini: " << t->deskripsi << endl;
    cout << "Deskripsi baru [Enter jika tidak diubah]: ";
    string deskripsiBaru;
    getline(cin, deskripsiBaru);
    if (!deskripsiBaru.empty()) {
        t->deskripsi = deskripsiBaru;
    }
    
    cout << "Nominal saat ini: " << t->jumlah << endl;
    cout << "Nominal baru [0 atau negatif jika tidak diubah]: ";
    double jumlahBaru;
    cin >> jumlahBaru;
    cin.ignore(10000, '\n');
    if (jumlahBaru > 0) {
        t->jumlah = jumlahBaru;
    }
    
    cout << "Data transaksi berhasil diperbarui!" << endl;
}

void hapusTransaksi() {
    if (daftarTransaksi == nullptr) return;
    if (jumlahData == 0) {
        cout << "Belum ada transaksi untuk dihapus." << endl;
        return;
    }
    
    lihatRiwayat();
    
    int idHapus;
    cout << "\n--- HAPUS TRANSAKSI ---" << endl;
    cout << "Masukkan ID transaksi yang ingin dihapus: ";
    cin >> idHapus;
    cin.ignore(10000, '\n');
    
    int indexHapus = -1;
    for (int i = 0; i < jumlahData; i++) {
        if (daftarTransaksi[i].id == idHapus) {
            indexHapus = i;
            break;
        }
    }
    
    if (indexHapus == -1) {
        cout << "Error: ID transaksi tidak ditemukan!" << endl;
        return;
    }
    
    char konfirmasi;
    cout << "Yakin ingin menghapus transaksi ID " << idHapus << "? (y/n): ";
    cin >> konfirmasi;
    cin.ignore(10000, '\n');
    
    if (konfirmasi != 'y' && konfirmasi != 'Y') {
        cout << "Penghapusan dibatalkan." << endl;
        return;
    }
    
    for (int i = indexHapus; i < jumlahData - 1; i++) {
        daftarTransaksi[i] = daftarTransaksi[i + 1];
    }
    jumlahData--;
    
    cout << "Transaksi ID " << idHapus << " berhasil dihapus!" << endl;
}

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

void bacaDariFile() {
    ifstream file(FILENAME);
    if (!file) {
        cout << "Info: File '" << FILENAME << "' tidak ditemukan. Memulai dari database baru." << endl;
        return;
    }
    
    string line;
    jumlahData = 0;
    
    while (getline(file, line) && jumlahData < MAX_TRANSAKSI) {
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
            
            t->id = stoi(data[0]);
            t->tanggal = data[1];
            t->kategori = data[2];
            t->deskripsi = data[3];
            t->jumlah = stod(data[4]);
            
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
