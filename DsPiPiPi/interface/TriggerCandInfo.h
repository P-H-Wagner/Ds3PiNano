#ifndef controlChannel_DsPiPiPi_TriggerCandInfo_h
#define controlChannel_DsPiPiPi_TriggerCandInfo_h

struct TriggerCandInfo {

    int piIdx;
    int trgObjIdx;

    int mu7_ip4;
    int prescale_mu7_ip4;

    int mu8_ip3;
    int prescale_mu8_ip3;

    int mu8_ip5;
    int prescale_mu8_ip5;

    int mu8p5_ip3p5;
    int prescale_mu8p5_ip3p5;

    int mu8_ip6;
    int prescale_mu8_ip6;

    int mu9_ip4;
    int prescale_mu9_ip4;

    int mu9_ip5;
    int prescale_mu9_ip5;

    int mu9_ip6;
    int prescale_mu9_ip6;

    int mu10p5_ip3p5;
    int prescale_mu10p5_ip3p5;

    int mu12_ip6;
    int prescale_mu12_ip6;

    int filter_mu7_4;
    int filter_mu8_3;
    int filter_mu8_5;
    int filter_mu8p5_3p5;
    int filter_mu8_6;
    int filter_mu9_4;
    int filter_mu9_5;
    int filter_mu9_6;
    int filter_mu10p5_3p5;
    int filter_mu12_6;
};

#endif
