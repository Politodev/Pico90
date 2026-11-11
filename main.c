#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "cyw43.h"

#include "config.h"

int main(void)
{
    stdio_init_all();
    sleep_ms(2000);

    printf("\n=============================\n");
    printf(" PICO-90DNS-ROUTER\n");
    printf(" Raspberry Pi Pico 2 W\n");
    printf("=============================\n");

    // Initialisation Wi-Fi
    if (cyw43_arch_init()) {
        printf("[ERREUR] Initialisation Wi-Fi impossible.\n");
        return 1;
    }

    printf("[OK] Module Wi-Fi initialise.\n");

    // Activation du point d'acces
    cyw43_arch_enable_ap_mode(
        AP_SSID,
        AP_PASSWORD,
        CYW43_AUTH_WPA2_AES_PSK
    );

    printf("[OK] Point d'acces active : %s\n", AP_SSID);

    // Activation du mode client Wi-Fi
    cyw43_arch_enable_sta_mode();

    printf("[OK] Mode client active.\n");

    // Connexion asynchrone a la box
    int result = cyw43_arch_wifi_connect_async(
        WIFI_SSID,
        WIFI_PASSWORD,
        CYW43_AUTH_WPA2_AES_PSK
    );

    if (result == 0) {
        printf("[INFO] Tentative de connexion a la box...\n");
    } else {
        printf("[ERREUR] Impossible de lancer la connexion : %d\n", result);
    }

    // Surveillance permanente
    while (true) {
        int status = cyw43_tcpip_link_status(
    &cyw43_state,
    CYW43_ITF_STA
);

switch (status) {
    case CYW43_LINK_DOWN:
        printf("[WIFI] Deconnecte\n");
        break;

    case CYW43_LINK_JOIN:
        printf("[WIFI] Connecte au Wi-Fi, attente IP\n");
        break;

    case CYW43_LINK_NOIP:
        printf("[WIFI] Connecte, pas encore d'adresse IP\n");
        break;

    case CYW43_LINK_UP:
        printf("[WIFI] INTERNET : connexion Wi-Fi avec IP !\n");
        break;

    case CYW43_LINK_FAIL:
        printf("[ERREUR] Echec de connexion\n");
        break;

    case CYW43_LINK_NONET:
        printf("[ERREUR] Reseau introuvable\n");
        break;

    case CYW43_LINK_BADAUTH:
        printf("[ERREUR] Mot de passe incorrect\n");
        break;

    default:
        printf("[WIFI] Etat inconnu : %d\n", status);
        break;
}

        printf("[WIFI] Etat connexion box : %d\n", status);

        sleep_ms(3000);
    }

    return 0;
}