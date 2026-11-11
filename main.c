
#include <stdio.h>

#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "cyw43.h"

#include "lwip/ip4_addr.h"
#include "lwip/init.h"

#include "dhcpserver.h"
#include "config.h"

static dhcp_server_t dhcp_server;

int main(void)
{
    stdio_init_all();
    sleep_ms(2000);

    printf("\n=============================\n");
    printf(" PICO-90DNS-ROUTER V2\n");
    printf(" Raspberry Pi Pico 2 W\n");
    printf("=============================\n");

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

    printf("[OK] Point d'acces actif : %s\n", AP_SSID);

    // Configuration de l'adresse du reseau AP
    ip4_addr_t gw;
    ip4_addr_t mask;

    IP4_ADDR(&gw, 192, 168, 4, 1);
    IP4_ADDR(&mask, 255, 255, 255, 0);

    // Demarrage du serveur DHCP sur l'interface AP
    dhcp_server_init(
        &dhcp_server,
        &cyw43_state.netif[CYW43_ITF_AP],
        &gw,
        &mask
    );

    printf("[DHCP] Serveur DHCP demarre.\n");
    printf("[DHCP] Passerelle : 192.168.4.1\n");
    printf("[DHCP] Plage prevue : 192.168.4.16 a 192.168.4.23\n");

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
        printf("[ERREUR] Connexion impossible : %d\n", result);
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
                printf("[WIFI] Connexion Wi-Fi en cours\n");
                break;

            case CYW43_LINK_NOIP:
                printf("[WIFI] En attente d'adresse IP\n");
                break;

            case CYW43_LINK_UP:
                printf("[WIFI] Box connectee avec IP !\n");
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
                printf("[WIFI] Etat : %d\n", status);
                break;
        }

        sleep_ms(5000);
    }

    return 0;
}
