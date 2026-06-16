#include <iostream>
#include <string>

using namespace std;

// ==========================================
// 1. STRUCT & ARRAY (Tanggung Jawab: Nia)
// ==========================================
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

// ==========================================
// SEARCHING ALGORITHM (Tanggung Jawab: Haidar, Farel, Fernanda)
// ==========================================
Restoran* cariRestoran(string namaCari) {
    Restoran* temp = headRestoran;
    while (temp != nullptr) {
        if (temp->nama == namaCari) {
            return temp;
        }
        temp = temp->next;
    }
    return nullptr;
}

// ==========================================
// SORTING ALGORITHM - Best Seller (Farel & Fernanda)
// ==========================================
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

// ==========================================
// QUEUE FOR BOOKING (Tanggung Jawab: Haidar & Nia)
// ==========================================
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

// ==========================================
// STACK FOR CANCEL BOOKING (Tanggung Jawab: Haidar)
// ==========================================
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
    inisialisasiData();
    int pilihanRole, pilihanMenu;
    string username, password;

    cout << "=========================================\n";
    cout << "  SISTEM RESERVASI RESTORAN (STRUKDAT)  \n";
    cout << "=========================================\n";
    
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
                    cout << "5. Logout\nPilihan: ";
                    cin >> menuAdmin;

                    if (menuAdmin == 1) {
                        int idR; string menuBaru; int hg;
                        tampilkanSemuaRestoran();
                        cout << "Pilih ID Restoran: "; cin >> idR;
                        cout << "Nama Menu Baru/Update: "; cin.ignore(); getline(cin, menuBaru);
                        cout << "Harga: "; cin >> hg;
                        tambahMenuKeRestoran(idR, menuBaru, hg, 0);
                        cout << "Menu Berhasil Diperbarui.\n";
                    } 
                    else if (menuAdmin == 2) {
                        if (isQueueEmpty()) {
                            cout << "Tidak ada antrean pembayaran.\n";
                        } else {
                            Booking b = dequeueBooking();
                            cout << "Memproses Pembayaran ID: " << b.idBooking << " atas nama " << b.namaPelanggan << "\n";
                            cout << "Total Tagihan: Rp" << b.totalBayar << " -> [LUNAS]\n";
                        }
                    }
                    else if (menuAdmin == 3) {
                        cancelBookingTerakhir();
                    }
                    else if (menuAdmin == 4) {
                        tampilkanRiwayatPembatalan();
                    }
                } while (menuAdmin != 5);
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
                        
                        // Menampilkan menu sebelum disortir
                        cout << "[Menu Original]:\n";
                        for(int i=0; i<r->jumlahMenu; i++) {
                            cout << "- " << r->daftarMenu[i].nama << " (Rp" << r->daftarMenu[i].harga << ")\n";
                        }

                        // Menampilkan Best Seller menggunakan sorting (Tanggung jawab Farel/Fernanda)
                        urutkanMenuBestSeller(r->daftarMenu, r->jumlahMenu);
                        cout << "\n[Tampilan Depan - Best Seller Menu]:\n";
                        cout << "★ " << r->daftarMenu[0].nama << " (Terjual: " << r->daftarMenu[0].terjual << " pcs) ★\n";
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
                    } else {
                        cout << "Restoran salah.\n";
                    }
                }
            } while (menuUser != 4);
        }
    } while (pilihanRole != 3);

    cout << "\nTerima kasih telah menggunakan sistem kami!\n";
    return 0;
}