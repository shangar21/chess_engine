#include "types.h"


U64 rookMasks[64] = {
    72340172838076926,    144680345676153597,   289360691352306939,
    578721382704613623,   1157442765409226991,  2314885530818453727,
    4629771061636907199,  9259542123273814143,  72340172838141441,
    144680345676217602,   289360691352369924,   578721382704674568,
    1157442765409283856,  2314885530818502432,  4629771061636939584,
    9259542123273813888,  72340172854657281,    144680345692602882,
    289360691368494084,   578721382720276488,   1157442765423841296,
    2314885530830970912,  4629771061645230144,  9259542123273748608,
    72340177082712321,    144680349887234562,   289360695496279044,
    578721386714368008,   1157442769150545936,  2314885534022901792,
    4629771063767613504,  9259542123257036928,  72341259464802561,
    144681423712944642,   289361752209228804,   578722409201797128,
    1157443723186933776,  2314886351157207072,  4629771607097753664,
    9259542118978846848,  72618349279904001,    144956323094725122,
    289632270724367364,   578984165983651848,   1157687956502220816,
    2315095537539358752,  4629910699613634624,  9259541023762186368,
    143553341945872641,   215330564830528002,   358885010599838724,
    645993902138460168,   1220211685215703056,  2368647251370188832,
    4665518383679160384,  9259260648297103488,  18302911464433844481,
    18231136449196065282, 18087586418720506884, 17800486357769390088,
    17226286235867156496, 16077885992062689312, 13781085504453754944,
    9187484529235886208};

U64 bishopMasks[64] = {
    9241421688590303744,  36099303471056128,   141012904249856,
    550848566272,         6480472064,          1108177604608,
    283691315142656,      72624976668147712,   4620710844295151618,
    9241421688590368773,  36099303487963146,   141017232965652,
    1659000848424,        283693466779728,     72624976676520096,
    145249953336262720,   2310355422147510788, 4620710844311799048,
    9241421692918565393,  36100411639206946,   424704217196612,
    72625527495610504,    145249955479592976,  290499906664153120,
    1155177711057110024,  2310355426409252880, 4620711952330133792,
    9241705379636978241,  108724279602332802,  145390965166737412,
    290500455356698632,   580999811184992272,  577588851267340304,
    1155178802063085600,  2310639079102947392, 4693335752243822976,
    9386671504487645697,  326598935265674242,  581140276476643332,
    1161999073681608712,  288793334762704928,  577868148797087808,
    1227793891648880768,  2455587783297826816, 4911175566595588352,
    9822351133174399489,  1197958188344280066, 2323857683139004420,
    144117404414255168,   360293502378066048,  720587009051099136,
    1441174018118909952,  2882348036221108224, 5764696068147249408,
    11529391036782871041, 4611756524879479810, 567382630219904,
    1416240237150208,     2833579985862656,    5667164249915392,
    11334324221640704,    22667548931719168,   45053622886727936,
    18049651735527937};

U64 queenMasks[64] = {
    9313761861428380670,  180779649147209725,   289501704256556795,
    578721933553179895,   1157442771889699055,  2314886638996058335,
    4630054752952049855,  9332167099941961855,  4693051017133293059,
    9386102034266586375,  325459994840333070,   578862399937640220,
    1157444424410132280,  2315169224285282160,  4702396038313459680,
    9404792076610076608,  2382695595002168069,  4765391190004401930,
    9530782384287059477,  614821794359483434,   1157867469641037908,
    2387511058326581416,  4775021017124823120,  9550042029937901728,
    1227517888139822345,  2455035776296487442,  4910072647826412836,
    9820426766351346249,  1266167048752878738,  2460276499189639204,
    4920271519124312136,  9840541934442029200,  649930110732142865,
    1299860225776030242,  2600000831312176196,  5272058161445620104,
    10544115227674579473, 2641485286422881314,  5210911883574396996,
    10421541192660455560, 361411684042608929,   722824471891812930,
    1517426162373248132,  3034571949281478664,  6068863523097809168,
    12137446670713758241, 5827868887957914690,  11583398706901190788,
    287670746360127809,   575624067208594050,   1079472019650937860,
    2087167920257370120,  4102559721436811280,  8133343319517438240,
    16194909420462031425, 13871017173176583298, 18303478847064064385,
    18232552689433215490, 18090419998706369540, 17806153522019305480,
    17237620560088797200, 16100553540994408480, 13826139127340482880,
    9205534180971414145};