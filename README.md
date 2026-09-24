# Wii U UI Latency Test

Aplikasi homebrew Wii U sederhana untuk membandingkan waktu pembaruan gambar
di TV dan Wii U GamePad.

## Prasyarat

- Docker Desktop (macOS atau Windows) atau Docker Engine (Linux) yang sedang
  berjalan.
- `make` untuk perintah singkat di Linux dan macOS. Windows dapat langsung
  memakai perintah Docker sehingga tidak memerlukan `make`.

## Build

Jalankan perintah dari root repositori.

### Linux

```sh
make docker
```

### macOS

```sh
make docker
```

### Windows PowerShell

```powershell
docker build --target artifact --output type=local,dest=build .
```

### Windows Command Prompt

```bat
docker build --target artifact --output type=local,dest=build .
```

Semua perintah menghasilkan berkas berikut:

```text
build/drc-latency-test.rpx
```

## Menjalankan di Wii U

1. Gunakan lingkungan homebrew Wii U yang mendukung aplikasi RPX dan akses ke
   Homebrew Launcher atau Homebrew Menu.
2. Buat direktori `wiiu/apps/drc-latency-test/` pada kartu SD.
3. Salin `build/drc-latency-test.rpx` ke direktori tersebut. Nama berkas dapat
   dipertahankan, sehingga jalurnya menjadi
   `sd:/wiiu/apps/drc-latency-test/drc-latency-test.rpx`.
4. Masukkan kartu SD ke Wii U, buka Homebrew Launcher atau Homebrew Menu, lalu
   jalankan aplikasi.

Ikuti petunjuk instalasi lingkungan homebrew yang digunakan. Proyek ini hanya
menyediakan aplikasi RPX dan tidak memasang atau mengeksploitasi homebrew.

## Prosedur pengukuran

Rekam TV dan GamePad **bersamaan dalam satu video**. Kedua layar harus terlihat
pada frame kamera yang sama agar nilai `FRAMECOUNT` dapat dibandingkan langsung.
Selisih nilai pada frame video yang sama menunjukkan selisih pembaruan tampilan
antara TV dan GamePad.

Tampilan aplikasi memuat:

- `FPS`: laju frame yang dihitung selama interval pengukuran terakhir.
- `FRAMECOUNT`: jumlah frame yang sudah digambar sejak aplikasi mulai berjalan.
- `TIMESTAMP`: waktu yang berlalu sejak aplikasi mulai berjalan, dalam format
  `HH:MM:SS.mmm`.

## Batasan versi pertama

Versi pertama sengaja hanya menampilkan data pengukuran. Belum ada fungsi
reset, jeda, animasi, atau mode tambahan.
