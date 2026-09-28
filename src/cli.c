#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <csp/csp.h>
#include <csp/drivers/eth_linux.h>
#include <csp/drivers/can_socketcan.h>
#include <csp/interfaces/csp_if_zmqhub.h>
#include "si-csp-utils/cli.h"

static unsigned int eth_addr = 0;
static unsigned int eth_mask = 12;
static unsigned int can_addr = 143;
static unsigned int can_mask = 12;
static char *eth_dev = "eth0";
static unsigned int zmq_addr = 0;
static unsigned int zmq_mask = 8;
static char *zmq_proxy = "localhost";


static struct option itf_options[] = {
    {"eth_addr", required_argument, 0, 1},
    {"eth_mask", required_argument, 0, 2},
    {"eth_dev", required_argument, 0, 5},
    {"can_addr", required_argument, 0, 3},
    {"can_mask", required_argument, 0, 4},
    {"zmq_addr", required_argument, 0, 7},
    {"zmq_mask", required_argument, 0, 8},
    {"zmq_proxy", required_argument, 0, 9},
    {"help", no_argument, 0, 'h'},
    {0, 0, 0, 0}};

static void print_usage(char *prog_name) {
    printf("Usage: %s [options]\n", prog_name);
    printf("Options:\n");
    printf("  --eth_addr <addr>       Set Ethernet interface address (default: %u)\n", eth_addr);
    printf("  --eth_mask <mask>       Set Ethernet interface netmask (default: %u)\n", eth_mask);
    printf("  --eth_dev <device>      Set Ethernet device name (default: %s)\n", eth_dev);
    printf("  --can_addr <addr>       Set CAN interface address (default: %u)\n", can_addr);
    printf("  --can_mask <mask>       Set CAN interface netmask (default: %u)\n", can_mask);
    printf("  --zmq_addr <addr>       Set ZMQ interface address (default: %u)\n", zmq_addr);
    printf("  --zmq_mask <mask>       Set ZMQ interface netmask (default: %u)\n", zmq_mask);
    printf("  --zmq_proxy <proxy>     Set ZMQ proxy address (default: %s)\n", zmq_proxy);
    printf("  -h, --help              Show this help message\n");
}

static void parse_cli_args(int argc, char **argv) {
    int opt;
    int option_index = 0;

    while ((opt = getopt_long(argc, argv, "h", itf_options, &option_index)) != -1) {
        switch (opt) {
            case 1:
                eth_addr = (unsigned int)atoi(optarg);
                break;
            case 2:
                eth_mask = (unsigned int)atoi(optarg);
                break;
            case 3:
                can_addr = (unsigned int)atoi(optarg);
                break;
            case 4:
                can_mask = (unsigned int)atoi(optarg);
                break;
            case 5:
                eth_dev = optarg;
                break;
            case 7:
                zmq_addr = (unsigned int)atoi(optarg);
                break;
            case 8:
                zmq_mask = (unsigned int)atoi(optarg);
                break;
            case 9:
                zmq_proxy = optarg;
                break;
            case 'h':
            default:
                print_usage(argv[0]);
                exit(1);
        }
    }
}

int si_cli_configure_csp_interfaces(int argc, char **argv) {
    parse_cli_args(argc, argv);

    if(can_addr) {
        csp_iface_t *can_itf;
        if (CSP_ERR_NONE != csp_can_socketcan_open_and_add_interface("can0", "CAN", can_addr, 1000000, 1, &can_itf))
        {
            fprintf(stderr, "Cannot instanciate CAN interface!\n");
        } else {
            can_itf->is_default = 1;
            can_itf->addr = can_addr;
            can_itf->netmask = can_mask;
        }
    }
    if(eth_addr) {
        csp_iface_t *eth_itf;
        if (CSP_ERR_NONE != csp_eth_init(eth_dev, "ETH", 1200, eth_addr, 0, &eth_itf))
        {
            fprintf(stderr, "Cannot instanciate ETH interface!\n");
        } else {
            eth_itf->is_default = 1;
            eth_itf->addr = eth_addr;
            eth_itf->netmask = eth_mask;
        }
    }
    if(zmq_addr) {
        csp_iface_t *zmq_itf;
            if (CSP_ERR_NONE != csp_zmqhub_init_filter2((const char *)"ZMQ", zmq_proxy, zmq_addr, zmq_mask, 1, &zmq_itf, 0, 6000, 7000))
        {
            fprintf(stderr, "Cannot instanciate ZMQ interface!\n");
        } else {
            zmq_itf->is_default = 1;
            zmq_itf->addr = zmq_addr;
            zmq_itf->netmask = zmq_mask;
        }
    }

    return 0;
}
