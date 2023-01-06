//
// Created by nflet on 1/2/2023.
//
#include "UsefulConstants.h"

extern const unsigned long long FileMasks[8] = {

        0x101010101010101,
        0x202020202020202,
        0x404040404040404,
        0x808080808080808,
        0x1010101010101010,
        0x2020202020202020,
        0x4040404040404040,
        0x8080808080808080,

};

extern const unsigned long long RankMasks[8] = {
        0xff,
        0xff00,
        0xff0000,
        0xff000000,
        0xff00000000,
        0xff0000000000,
        0xff000000000000,
        0xff00000000000000
};

//generated from initializeBishopTargetCountLookup()
extern const int BishopTargetCount[64] = {
        6, 5, 5, 5, 5, 5, 5, 6,
        5, 5, 5, 5, 5, 5, 5, 5,
        5, 5, 7, 7, 7, 7, 5, 5,
        5, 5, 7, 9, 9, 7, 5, 5,
        5, 5, 7, 9, 9, 7, 5, 5,
        5, 5, 7, 7, 7, 7, 5, 5,
        5, 5, 5, 5, 5, 5, 5, 5,
        6, 5, 5, 5, 5, 5, 5, 6
};

//generated from initializeRookTargetCountLookup()
extern const int RookTargetCount[64] = {
        12, 11, 11, 11, 11, 11, 11, 12,
        11, 10, 10, 10, 10, 10, 10, 11,
        11, 10, 10, 10, 10, 10, 10, 11,
        11, 10, 10, 10, 10, 10, 10, 11,
        11, 10, 10, 10, 10, 10, 10, 11,
        11, 10, 10, 10, 10, 10, 10, 11,
        11, 10, 10, 10, 10, 10, 10, 11,
        12, 11, 11, 11, 11, 11, 11, 12
};

//generated from findMagicNumbers()
extern const unsigned long long BishopMagics[64] = {
        1213929573326976,
        600113481890409472,
        577658137729826816,
        581263702564339872,
        73188029430236424,
        90637150651090960,
        72136776661213188,
        4901359538151694336,
        74938864370816,
        4521200681292288,
        4578933421391874,
        144119603311166592,
        5260512538353734656,
        146772167053411360,
        36596153630073112,
        288230943096321152,
        1447336199325760,
        1196337647452227,
        4613400037066670592,
        9077585195573248,
        288511886025555968,
        70514778309656,
        2450802630829088768,
        2900883622540214833,
        2256198934218752,
        649189117292709925,
        1442278914805792804,
        18018797797574672,
        4629981960951840768,
        1441232145141006593,
        578722447755378976,
        4785358105616544,
        1178951344198848,
        74328102732301313,
        577024458173318020,
        2310390623668144640,
        290279659676160,
        21401040352060032,
        76562294530180096,
        1185010236606448648,
        218714891723227136,
        4612814435453764224,
        35270305058848,
        27030531581417472,
        5307511960723653120,
        27031012877860929,
        148621295983005760,
        9570703359688968,
        565566125899848,
        6917811606980133004,
        162134126500512002,
        692035534850,
        1153203804489974274,
        2305879851478941728,
        2287019652827144,
        166916868806935081,
        602533513930890,
        4611721481981272128,
        4556376756015105,
        2314855156308640264,
        864691403888804096,
        19140642586757184,
        2295789205864968,
        2359922497551352192

};


//generated from findMagicNumbers()
extern const unsigned long long RookMagics[64] = {
        36029071906791552,
        1171006272128950272,
        5224210760980236672,
        5944760304490513538,
        144119594712829984,
        2377909403639743488,
        4791830553294799360,
        72068881841259264,
        11540615787446336,
        4972888783367180292,
        146648600321998872,
        1238771415721713920,
        2533309151315200,
        75998346866720784,
        11540491258560768,
        2306406015031313042,
        36037867991993408,
        4503875579027458,
        144399962124337172,
        576480543684821024,
        83599167728914948,
        6992120996115513346,
        671037448284930052,
        36092568706286212,
        36029076194001024,
        35194036814144,
        9007475207440384,
        4618451326360096768,
        5188710863146590212,
        2306124522845372418,
        333302673490051752,
        2416199350618178561,
        18084767798919296,
        612560055509716996,
        4508032035725312,
        433542975373447200,
        2306406098384458768,
        37717664067551360,
        4792392970756163595,
        2306124485280923778,
        36028934462652416,
        148618925188464640,
        4619039556493180948,
        1409642636967945,
        150870609142808592,
        4611756404620394784,
        2233400033296,
        11544873173909521,
        4647785184802898560,
        360323155636520000,
        2918354686224384256,
        4692786271137829376,
        111466839624122496,
        613615518486103041,
        36318516216832,
        576747206305120768,
        4900198008069685378,
        70927092088850,
        18691833988161,
        2310346746348177665,
        576742261640135185,
        5191805962616440833,
        585485827231449220,
        1153207390808580418
};


