# cub3D

42 okulunun cub3D projesi. Basit bir raycasting motoru kullanarak birinci şahıs 3D görünüm oluşturur. MiniLibX kütüphanesi ile pencere yönetimi ve grafik çizimi yapılır.

## Ne yapıyor?

`.cub` uzantılı bir harita dosyası alıyor, içindeki duvar texture yollarını, zemin/tavan renklerini ve harita verisini parse edip ekrana 3D olarak yansıtıyor. Oyuncu WASD ile hareket eder, ok tuşlarıyla etrafına bakar.

## Gereksinimler

Linux üzerinde çalışır. Şu paketlerin kurulu olması gerekiyor:

```bash
sudo apt update
sudo apt install -y build-essential xorg libxext-dev libbsd-dev
```

MiniLibX `lib/mlx_linux` altında olmalı. Yoksa `make` otomatik çeker:

```bash
mkdir -p lib
git clone https://github.com/42Paris/minilibx-linux.git lib/mlx_linux
```

## Derleme ve çalıştırma

```bash
make
./cub3D maps/demo.cub
```

Farklı bir harita denemek için:

```bash
./cub3D maps/maze.cub
```

## Kontroller

| Tuş | İşlev |
|-----|-------|
| W | İleri |
| S | Geri |
| A | Sola kayma |
| D | Sağa kayma |
| ← | Sola dönme |
| → | Sağa dönme |
| ESC | Çıkış |

## Harita formatı (.cub)

```
NO ./textures/north.xpm
SO ./textures/south.xpm
WE ./textures/west.xpm
EA ./textures/east.xpm
F 60,60,60
C 30,60,90

111111111111
100000000001
101111011101
1000N0010001
101011010101
100000000001
111111111111
```

- `NO/SO/WE/EA`: Duvar texture dosya yolları (.xpm)
- `F`: Zemin rengi (R,G,B — 0-255 arası)
- `C`: Tavan rengi (R,G,B — 0-255 arası)
- `1`: Duvar, `0`: Boş alan, `N/S/E/W`: Oyuncu başlangıç pozisyonu ve yönü
- Harita tamamen duvarlarla çevrili olmalı

## Proje yapısı

```
srcs/
├── main.c                 # Giriş noktası, mlx loop başlatma
├── map/
│   ├── parser.c           # .cub dosyası okuma ve kontrol
│   ├── parser_file.c      # Dosya okuma, satır ayırma
│   ├── parser_header.c    # NO/SO/WE/EA/F/C parse etme
│   ├── parser_elements.c  # Harita satırı tanıma, ön kontrol
│   ├── parser_map.c       # Grid oluşturma, spawn bulma
│   ├── parser_validate.c  # Flood fill ile harita kapalılık kontrolü
│   └── parser_utils.c     # Yardımcı fonksiyonlar
├── player/
│   ├── input.c            # Tuş basma/bırakma olayları
│   └── move.c             # Hareket ve çarpışma kontrolü
├── raycast/
│   ├── render.c           # DDA raycasting ve çizim
│   └── texture.c          # XPM texture yükleme
└── system/
    ├── init.c             # Oyun yapısı ve MLX başlatma
    ├── cleanup.c          # Bellek temizleme
    └── errors.c           # Hata mesajı yazdırma
```

## Özellikler

- DDA tabanlı raycasting ile texture'lı duvar render'ı
- WASD hareket + ok tuşu ile kamera dönüşü
- Bounding box tabanlı çarpışma kontrolü (duvar içine girme engeli)
- Tam `.cub` parser: header, RGB, harita validasyonu
- Flood fill ile haritanın duvarlarla kapalı olup olmadığının kontrolü
- Hatalı dosya uzantısı ve geçersiz renk değeri (overflow) kontrolü
