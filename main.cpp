#include <iostream>
#include <string>
#include <algorithm> // Ditambahkan untuk fungsi transform (case-insensitive)
#include <fstream>   // Ditambahkan untuk file handling (penyimpanan data)
#include <sstream>   // Ditambahkan untuk parsing baris file (stringstream)

using namespace std;

// ================== NAMA FILE PENYIMPANAN ==================
const string FILE_RESTORAN = "data_restoran.txt";
const string FILE_CANCEL   = "data_cancel.txt";
const string FILE_COUNTER  = "data_counter.txt";

// Fungsi pembantu untuk mengubah string menjadi huruf kecil semua
string toLowerCase(string str) {
    transform(str.begin(), str.end(), str.begin(), ::tolower);
    return str;
}

// 1. STRUCT & ARRAY (Tanggung Jawab: Nia)

struct Menu {
    string nama;
    int harga;
    int terjual;
};

struct Booking {
    int idBooking;
    string namaPelanggan;
    string waktu;
    int jumlahKursi;
    string jenisPaket;
    int totalBayar;
};

struct Restoran {
    int id;
    string nama;
    Menu daftarMenu[3];
    int jumlahMenu;
    Restoran* next;
};

struct CancelNode {
    Booking data;
    CancelNode* next;
};

Restoran* headRestoran = nullptr;

void tambahRestoran(int id, string nama) {
    Restoran* restoBaru = new Restoran;
    restoBaru->id = id;
    restoBaru->nama = nama;
    restoBaru->jumlahMenu = 0;
    restoBaru->next = nullptr;

    if (headRestoran == nullptr) {
        headRestoran = restoBaru;
    } else {
        Restoran* temp = headRestoran;
        while (temp->next != nullptr) {
            temp = temp->next;
        }
        temp->next = restoBaru;
    }
}

// Backend: Update Menu Restoran
void tambahMenuKeRestoran(int idResto, string namaMenu, int harga, int terjual) {
    Restoran* temp = headRestoran;
    while (temp != nullptr) {
        if (temp->id == idResto) {
            if (temp->jumlahMenu < 3) {
                temp->daftarMenu[temp->jumlahMenu] = {namaMenu, harga, terjual};
                temp->jumlahMenu++;
            }
            return;
        }
        temp = temp->next;
    }
}

// SEARCHING ALGORITHM (Tanggung Jawab: Haidar, Farel, Fernanda)
Restoran* cariRestoran(string namaCari) {
    Restoran* temp = headRestoran;
    while (temp != nullptr) {
        if (toLowerCase(temp->nama) == toLowerCase(namaCari)) {
            return temp;
        }
        temp = temp->next;
    }
    return nullptr;
}

// SORTING ALGORITHM - Best Seller (Farel & Fernanda)
void urutkanMenuBestSeller(Menu arr[], int n) {
    // Menggunakan Bubble Sort secara Descending (Terbanyak ke Terkecil)
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j].terjual < arr[j + 1].terjual) {
                Menu temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

// QUEUE FOR BOOKING (Tanggung Jawab: Haidar & Nia)
const int MAX_QUEUE = 10;
Booking antreanBooking[MAX_QUEUE];
int frontQueue = -1;
int rearQueue = -1;
int idBookingCounter = 100;

bool isQueueFull() { return rearQueue == MAX_QUEUE - 1; }
bool isQueueEmpty() { return frontQueue == -1 || frontQueue > rearQueue; }

void enqueueBooking(string nama, string waktu, int kursi, string paket, int hargaMenu) {
    if (isQueueFull()) {
        cout << "[Sistem] Antrean penuh! Tidak bisa melakukan booking saat ini.\n";
        return;
    }
    if (frontQueue == -1) frontQueue = 0;

    rearQueue++;
    idBookingCounter++;
    antreanBooking[rearQueue] = {idBookingCounter, nama, waktu, kursi, paket, (hargaMenu * kursi)};
    cout << "[Sistem] Booking berhasil didaftarkan! ID Anda: " << idBookingCounter << "\n";
}

Booking dequeueBooking() {
    Booking dataKosong = {0, "", "", 0, "", 0};
    if (isQueueEmpty()) return dataKosong;

    Booking diproses = antreanBooking[frontQueue];
    frontQueue++;
    return diproses;
}

// STACK FOR CANCEL BOOKING (Tanggung Jawab: Haidar)
CancelNode* topStack = nullptr;

void pushCancel(Booking b) {
    CancelNode* nodeBaru = new CancelNode;
    nodeBaru->data = b;
    nodeBaru->next = topStack;
    topStack = nodeBaru;
}

void tampilkanRiwayatPembatalan() {
    if (topStack == nullptr) {
        cout << "Belum ada riwayat pembatalan.\n";
        return;
    }
    CancelNode* temp = topStack;
    cout << "\n=== RIWAYAT CANCEL BOOKING (STACK) ===\n";
    while (temp != nullptr) {
        cout << "- ID: " << temp->data.idBooking << " atas nama " << temp->data.namaPelanggan << " [Dibatalkan]\n";
        temp = temp->next;
    }
}

void cancelBookingTerakhir() {
    if (isQueueEmpty()) {
        cout << "[Sistem] Tidak ada antrean booking aktif untuk dibatalkan.\n";
        return;
    }
    Booking dicancel = antreanBooking[rearQueue];
    rearQueue--;
    pushCancel(dicancel);
    cout << "[Sistem] Booking ID " << dicancel.idBooking << " berhasil dicancel sebelum pembayaran.\n";
}

// FILE HANDLING (fstream) (Tanggung Jawab: Umbu)
// Format file data_restoran.txt (per baris):
// id|nama|jumlahMenu|namaMenu1,harga1,terjual1;namaMenu2,harga2,terjual2;...
//
// Format file data_cancel.txt (per baris, urutan dari TOP stack ke bawah):
// idBooking|namaPelanggan|waktu|jumlahKursi|jenisPaket|totalBayar
//
// Format file data_counter.txt (satu baris):
// idBookingCounter

void simpanDataRestoran() {
    ofstream fout(FILE_RESTORAN);
    if (!fout.is_open()) {
        cout << "[Sistem] Gagal membuka file untuk menyimpan data restoran.\n";
        return;
    }
    Restoran* temp = headRestoran;
    while (temp != nullptr) {
        fout << temp->id << "|" << temp->nama << "|" << temp->jumlahMenu << "|";
        for (int i = 0; i < temp->jumlahMenu; i++) {
            fout << temp->daftarMenu[i].nama << "," << temp->daftarMenu[i].harga
                 << "," << temp->daftarMenu[i].terjual;
            if (i != temp->jumlahMenu - 1) fout << ";";
        }
        fout << "\n";
        temp = temp->next;
    }
    fout.close();
}

// Memecah string berdasarkan delimiter (helper untuk parsing file)
void splitString(const string& str, char delim, string hasil[], int& jumlah, int maks) {
    jumlah = 0;
    stringstream ss(str);
    string token;
    while (getline(ss, token, delim) && jumlah < maks) {
        hasil[jumlah++] = token;
    }
}

void muatDataRestoran() {
    ifstream fin(FILE_RESTORAN);
    if (!fin.is_open()) {
        return; // File belum ada, biarkan sistem pakai data awal (inisialisasiData)
    }

    // Kosongkan linked list restoran yang ada sebelum memuat dari file
    while (headRestoran != nullptr) {
        Restoran* hapus = headRestoran;
        headRestoran = headRestoran->next;
        delete hapus;
    }

    string baris;
    bool adaData = false;
    while (getline(fin, baris)) {
        if (baris.empty()) continue;
        adaData = true;

        string kolom[4];
        int jumlahKolom = 0;
        splitString(baris, '|', kolom, jumlahKolom, 4);

        int id = stoi(kolom[0]);
        string nama = kolom[1];
        int jumlahMenu = stoi(kolom[2]);

        tambahRestoran(id, nama);

        if (jumlahKolom >= 4 && !kolom[3].empty()) {
            string menuMentah[3];
            int jumlahMenuMentah = 0;
            splitString(kolom[3], ';', menuMentah, jumlahMenuMentah, 3);

            for (int i = 0; i < jumlahMenuMentah; i++) {
                string bagianMenu[3];
                int jumlahBagian = 0;
                splitString(menuMentah[i], ',', bagianMenu, jumlahBagian, 3);
                if (jumlahBagian == 3) {
                    tambahMenuKeRestoran(id, bagianMenu[0], stoi(bagianMenu[1]), stoi(bagianMenu[2]));
                }
            }
        }
    }
    fin.close();
    if (adaData) {
        cout << "[Sistem] Data restoran berhasil dimuat dari file " << FILE_RESTORAN << ".\n";
    }
}

void simpanRiwayatPembatalan() {
    ofstream fout(FILE_CANCEL);
    if (!fout.is_open()) {
        cout << "[Sistem] Gagal membuka file untuk menyimpan riwayat pembatalan.\n";
        return;
    }
    CancelNode* temp = topStack;
    while (temp != nullptr) {
        fout << temp->data.idBooking << "|" << temp->data.namaPelanggan << "|"
             << temp->data.waktu << "|" << temp->data.jumlahKursi << "|"
             << temp->data.jenisPaket << "|" << temp->data.totalBayar << "\n";
        temp = temp->next;
    }
    fout.close();
}

void muatRiwayatPembatalan() {
    ifstream fin(FILE_CANCEL);
    if (!fin.is_open()) return;

    // File ditulis dari TOP ke bawah stack, sehingga jika kita push ulang
    // dengan urutan terbalik (dari baris terakhir ke baris pertama),
    // urutan stack yang asli akan terjaga.
    string baris;
    string semuaBaris[100];
    int jumlahBaris = 0;
    while (getline(fin, baris) && jumlahBaris < 100) {
        if (!baris.empty()) semuaBaris[jumlahBaris++] = baris;
    }
    fin.close();

    for (int i = jumlahBaris - 1; i >= 0; i--) {
        string kolom[6];
        int jumlahKolom = 0;
        splitString(semuaBaris[i], '|', kolom, jumlahKolom, 6);
        if (jumlahKolom == 6) {
            Booking b;
            b.idBooking = stoi(kolom[0]);
            b.namaPelanggan = kolom[1];
            b.waktu = kolom[2];
            b.jumlahKursi = stoi(kolom[3]);
            b.jenisPaket = kolom[4];
            b.totalBayar = stoi(kolom[5]);
            pushCancel(b);
        }
    }
    if (jumlahBaris > 0) {
        cout << "[Sistem] Riwayat pembatalan berhasil dimuat dari file " << FILE_CANCEL << ".\n";
    }
}

void simpanCounter() {
    ofstream fout(FILE_COUNTER);
    if (fout.is_open()) {
        fout << idBookingCounter << "\n";
        fout.close();
    }
}

void muatCounter() {
    ifstream fin(FILE_COUNTER);
    if (fin.is_open()) {
        int nilai;
        if (fin >> nilai) idBookingCounter = nilai;
        fin.close();
    }
}

// Dipanggil setiap kali ada perubahan data penting, supaya data selalu tersimpan otomatis
void simpanSemuaData() {
    simpanDataRestoran();
    simpanRiwayatPembatalan();
    simpanCounter();
}

void inisialisasiData() {
    tambahRestoran(1, "Resto Padang Minang");
    tambahMenuKeRestoran(1, "Rendang Daging", 25000, 150);
    tambahMenuKeRestoran(1, "Ayam Pop", 22000, 90);
    tambahMenuKeRestoran(1, "Gulai Tunjang", 27000, 200); // Best seller nomor 1 di resto 1

    tambahRestoran(2, "Shabu-Shabu Grill");
    tambahMenuKeRestoran(2, "Paket AYCE Premium", 150000, 80);
    tambahMenuKeRestoran(2, "Paket AYCE Regular", 99000, 120);
    tambahMenuKeRestoran(2, "Wagyu Plate", 50000, 45);
}

void tampilkanSemuaRestoran() {
    Restoran* temp = headRestoran;
    cout << "\n=== DAFTAR RESTORAN ===" << endl;
    while (temp != nullptr) {
        cout << temp->id << ". " << temp->nama << endl;
        temp = temp->next;
    }
}

int main() {
    muatDataRestoran();
    if (headRestoran == nullptr) {
        inisialisasiData();
        simpanDataRestoran();
    }
    muatRiwayatPembatalan();
    muatCounter();

    int pilihanRole, pilihanMenu;
    string username, password;

    cout << "=============================\n";
    cout << "  SISTEM RESERVASI RESTORAN  \n";
    cout << "=============================\n";

    do {
        cout << "\nPilih Akses Login:\n1. Admin\n2. User/Pelanggan\n3. Keluar Aplikasi\nPilihan: ";
        cin >> pilihanRole;

        if (pilihanRole == 1) {
            cout << "Masukkan Username Admin: "; cin >> username;
            cout << "Masukkan Password Admin: "; cin >> password;

            if (username == "admin" && password == "123") {
                int menuAdmin;
                do {
                    cout << "\n--- DASHBOARD BACKEND (ADMIN) ---\n";
                    cout << "1. Update Menu (Tambah Terjual Manual)\n";
                    cout << "2. Lihat & Proses Pembayaran Antrean Booking\n";
                    cout << "3. Cancel Antrean Booking Terakhir\n";
                    cout << "4. Lihat Riwayat Pembatalan (Stack)\n";
                    cout << "5. Tambah Restoran Baru\n";
                    cout << "6. Logout\nPilihan: ";
                    cin >> menuAdmin;

                    if (menuAdmin == 1) {
                        int idR; string menuBaru; int hg;
                        tampilkanSemuaRestoran();
                        cout << "Pilih ID Restoran: "; cin >> idR;
                        cout << "Nama Menu Baru/Update: "; cin.ignore(); getline(cin, menuBaru);
                        cout << "Harga: "; cin >> hg;
                        tambahMenuKeRestoran(idR, menuBaru, hg, 0);
                        simpanSemuaData();
                        cout << "Menu Berhasil Diperbarui dan disimpan ke file.\n";
                    }
                    else if (menuAdmin == 2) {
                        if (isQueueEmpty()) {
                            cout << "Tidak ada antrean pembayaran.\n";
                        } else {
                            Booking b = dequeueBooking();
                            cout << "Memproses Pembayaran ID: " << b.idBooking << " atas nama " << b.namaPelanggan << "\n";
                            cout << "Total Tagihan: Rp" << b.totalBayar << " -> [LUNAS]\n";
                            simpanSemuaData();
                        }
                    }
                    else if (menuAdmin == 3) {
                        cancelBookingTerakhir();
                        simpanSemuaData();
                    }
                    else if (menuAdmin == 4) {
                        tampilkanRiwayatPembatalan();
                    }
                    else if (menuAdmin == 5) {
                        int idRestoBaru; string namaRestoBaru;
                        cout << "Masukkan ID Restoran Baru: "; cin >> idRestoBaru;
                        cout << "Masukkan Nama Restoran Baru: "; cin.ignore(); getline(cin, namaRestoBaru);
                        tambahRestoran(idRestoBaru, namaRestoBaru);
                        simpanSemuaData();
                        cout << "Restoran " << namaRestoBaru << " Berhasil Ditambahkan dan disimpan ke file.\n";
                    }
                } while (menuAdmin != 6);
            } else {
                cout << "Login Admin Gagal!\n";
            }
        }
        else if (pilihanRole == 2) {
            // LOGIN USER
            int menuUser;
            do {
                cout << "\n--- DASHBOARD FRONT-END (USER) ---\n";
                cout << "1. Lihat Daftar Restoran\n";
                cout << "2. Cari Restoran & Lihat Menu (Termasuk Best Seller)\n";
                cout << "3. Booking Meja / Paket All You Can Eat\n";
                cout << "4. Kembali ke Menu Utama\nPilihan: ";
                cin >> menuUser;

                if (menuUser == 1) {
                    tampilkanSemuaRestoran();
                }
                else if (menuUser == 2) {
                    string cari;
                    cout << "Masukkan Nama Restoran yang dicari: ";
                    cin.ignore();
                    getline(cin, cari);

                    Restoran* r = cariRestoran(cari);
                    if (r != nullptr) {
                        cout << "\nRestoran ditemukan: " << r->nama << "\n";

                        cout << "[Menu Original]:\n";
                        for(int i=0; i<r->jumlahMenu; i++) {
                            cout << "- " << r->daftarMenu[i].nama << " (Rp" << r->daftarMenu[i].harga << ")\n";
                        }

                        // Jalankan fungsi sorting untuk mengurutkan array
                        urutkanMenuBestSeller(r->daftarMenu, r->jumlahMenu);

                        cout << "\n[Tampilan Depan - Best Seller Menu (Urutan Terlaris)]:\n";
                        for(int i = 0; i < r->jumlahMenu; i++) {
                            cout << i + 1 << ". " << r->daftarMenu[i].nama
                            << " (" << r->daftarMenu[i].terjual << " pcs)\n";
                        }
                        simpanDataRestoran(); // urutan array berubah, simpan ulang
                    } else {
                        cout << "Restoran tidak ditemukan.\n";
                    }
                }
                else if (menuUser == 3) {
                    string namaP, waktuP, paketP, namaR;
                    int kursiP;

                    tampilkanSemuaRestoran();
                    cout << "Ketik Nama Restoran tujuan: "; cin.ignore(); getline(cin, namaR);
                    Restoran* r = cariRestoran(namaR);

                    if (r != nullptr) {
                        cout << "Nama Anda: "; getline(cin, namaP);
                        cout << "Waktu Booking (Contoh 19:00): "; cin >> waktuP;
                        cout << "Jumlah Kursi: "; cin >> kursiP;
                        cout << "Jenis Paket (Reguler / AYCE): "; cin >> paketP;

                        // Mengambil estimasi harga dari menu pertama resto tersebut
                        int hargaEstimasi = r->daftarMenu[0].harga;
                        enqueueBooking(namaP, waktuP, kursiP, paketP, hargaEstimasi);
                        simpanCounter();
                    } else {
                        cout << "Restoran salah.\n";
                    }
                }
            } while (menuUser != 4);
        }
    } while (pilihanRole != 3);

    // Simpan seluruh data terakhir sebelum keluar aplikasi
    simpanSemuaData();
    cout << "\nSemua data telah disimpan ke file.\n";
    cout << "Terima kasih telah menggunakan sistem kami!\n";
    return 0;
}