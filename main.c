#include <stdio.h>

// Struct yapısı tanımlıyoruz.
struct daire {
    char ad[50];
    char soyad[50];
    float aidat;
};


// Yazılan isimlerin aynı olup olmadığını kontrol eden manuel bir fonksiyon yazıyoruz.
int metinAyniMi(char *metin1, char *metin2) {
    int i = 0;
    while (1) {
        // Eğer harfler farklıysa, metinler farklı olacağından 0 döndürür.
        if (metin1[i] != metin2[i]) {
            return 0;
        }
        // Eğer harfler aynıysa ve kelime sonuna gelindiyse 1 döndürür.
        if (metin1[i] == '\0') {
            return 1;
        }
        i++;
    }
}


// Kaynak metni alıp hedef metine kopyalayan manuel bir fonksiyon yazıyoruz.
void metinKopyala(char *hedef, char *kaynak) {
    int i = 0;
    // Kaynak olan kelimenin sonuna gelinene kadar harf harf taşıma yapılıyor.
    while (kaynak[i] != '\0') {
        hedef[i] = kaynak[i];
        i++;
    }
    hedef[i] = '\0';
}

// Listeye yeni kişi ekleyen veya var olanın aidatını güncelleyen bir fonksiyon yazıyoruz.
void ekle(struct daire dizi[], int *sayac, char *okunanAd, char *okunanSoyad, float okunanTutar) {
    int bulundu = 0;

    // Listede bu kişi var mı diye kontrol ediyoruz.
    for (int i = 0; i < *sayac; i++) {
        // Yazdığımız karşılaştırma fonksiyonunu kullanıyoruz. Burada hem ad kontrolü hem soyad kontrolü yapıyoruz.
        if (metinAyniMi(dizi[i].ad, okunanAd) == 1 && metinAyniMi(dizi[i].soyad, okunanSoyad) == 1) {
            // Kişinin aidat kısmına okunan yeni parayı ekler.
            dizi[i].aidat += okunanTutar;
            bulundu = 1;
            break;
        }
    }

    // Listede bu kişi yoksa yeni kayıt oluşturur.
    if (bulundu == 0) {
        if (*sayac < 15) {
            // Kendi yazdığımız kopyalama fonksiyonunu kullanıyoruz
            metinKopyala(dizi[*sayac].ad, okunanAd);
            metinKopyala(dizi[*sayac].soyad, okunanSoyad);
            dizi[*sayac].aidat = okunanTutar;

            (*sayac)++;
        } else {
            printf("Hata: Dizi kapasitesi dolu!\n");
        }
    }
}

// Verileri dosya içine yazan bir fonksiyon yazıyoruz.
void yazdir(struct daire dizi[], int sayac) {
    // toplam.txt adında bir dosya açar. Dosyayı w modunda açıyoruz çünkü içine veri yazılmasını istiyoruz.
    FILE *fp = fopen("toplam.txt", "w");
    // Dosyanın kontrolü yapılır.
    if (fp == NULL) {
        printf("toplam.txt dosyasi olusturulamadi.\n");
        return;
    }

    // Başlıkları koyuyoruz.
    fprintf(fp, "%-15s %-15s %s\n", "AD", "SOYAD", "TOPLAM TUTAR");
    // Burada verileri içine yazar.
    for (int i = 0; i < sayac; i++) {
        fprintf(fp, "%-15s %-15s %.2f\n", dizi[i].ad, dizi[i].soyad, dizi[i].aidat);
    }

    printf("Islem basarili. Sonuclar 'toplam.txt' dosyasina kaydedildi.\n");
    fclose(fp);
}

int main() {
    struct daire apartman[15];
    int kisiSayisi = 0;
    char tempAd[50];
    char tempSoyad[50];
    float tempTutar;
    // Kendi yazdığımız aidat.txt adındaki dosyayı açar. Dosyayı r modunda açıyoruz çünkü içine yazdıklarımızı okumasını istiyoruz.
    FILE *dosya = fopen("aidat.txt", "r");
    // Dosyanın kontrolü yapılır.
    if (dosya == NULL) {
        printf("HATA: 'aidat.txt' bulunamadi. Lutfen dosya yolunu kontrol edin.\n");
        return 1;
    }
    // Açtığımız dosya, dosya sonuna gelinene kadar (EOF) burada okunur.
    while (fscanf(dosya, "%s %s %f", tempAd, tempSoyad, &tempTutar) != EOF) {
        // Yazdığımız ekle fonksiyonunu kullanıyoruz.
        ekle(apartman, &kisiSayisi, tempAd, tempSoyad, tempTutar);
    }

    fclose(dosya);
    // Yazdığımız yazdir fonksiyonuyla sonuçları içine yazdırıyoruz.
    yazdir(apartman, kisiSayisi);

    return 0;
}