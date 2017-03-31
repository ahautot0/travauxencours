
#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <sstream> 
#include <hash_map>
using namespace std;


inline void sym5 (long long int nb, int& return1, bool& return2)   /*Cette fonction calcule les (S-1)! machines sym�triques qui impriment la m�me suite que nb*/
	{
	long long int quo, nx, min; 
	short i,j,k,nb20[24][10]={};

	quo=min=nb;
	
	for (i=9; quo>0; i--) {nb20[0][i] = quo%20; quo=quo/20;}

	nb20[1][0]=nb20[2][0]=nb20[3][0]=nb20[4][0]=nb20[5][0]=nb20[6][2]=nb20[7][2]=nb20[8][4]=nb20[9][6]=nb20[10][4]=nb20[11][6]=nb20[12][2]=nb20[13][2]=nb20[14][4]=nb20[15][6]=nb20[16][4]=nb20[17][6]=nb20[18][2]=nb20[19][2]=nb20[20][4]=nb20[21][6]=nb20[22][4]=nb20[23][6]=nb20[0][0];
	nb20[1][1]=nb20[2][1]=nb20[3][1]=nb20[4][1]=nb20[5][1]=nb20[6][3]=nb20[7][3]=nb20[8][5]=nb20[9][7]=nb20[10][5]=nb20[11][7]=nb20[12][3]=nb20[13][3]=nb20[14][5]=nb20[15][7]=nb20[16][5]=nb20[17][7]=nb20[18][3]=nb20[19][3]=nb20[20][5]=nb20[21][7]=nb20[22][5]=nb20[23][7]=nb20[0][1];
	nb20[1][2]=nb20[2][4]=nb20[3][6]=nb20[4][4]=nb20[5][6]=nb20[6][0]=nb20[7][0]=nb20[8][0]=nb20[9][0]=nb20[10][0]=nb20[11][0]=nb20[12][4]=nb20[13][6]=nb20[14][2]=nb20[15][2]=nb20[16][6]=nb20[17][4]=nb20[18][4]=nb20[19][6]=nb20[20][2]=nb20[21][2]=nb20[22][6]=nb20[23][4]=nb20[0][2];
	nb20[1][3]=nb20[2][5]=nb20[3][7]=nb20[4][5]=nb20[5][7]=nb20[6][1]=nb20[7][1]=nb20[8][1]=nb20[9][1]=nb20[10][1]=nb20[11][1]=nb20[12][5]=nb20[13][7]=nb20[14][3]=nb20[15][3]=nb20[16][7]=nb20[17][5]=nb20[18][5]=nb20[19][7]=nb20[20][3]=nb20[21][3]=nb20[22][7]=nb20[23][5]=nb20[0][3];
	nb20[1][6]=nb20[2][2]=nb20[3][2]=nb20[4][6]=nb20[5][4]=nb20[6][4]=nb20[7][6]=nb20[8][2]=nb20[9][2]=nb20[10][6]=nb20[11][4]=nb20[12][0]=nb20[13][0]=nb20[14][0]=nb20[15][0]=nb20[16][0]=nb20[17][0]=nb20[18][6]=nb20[19][4]=nb20[20][6]=nb20[21][4]=nb20[22][2]=nb20[23][2]=nb20[0][4];
	nb20[1][7]=nb20[2][3]=nb20[3][3]=nb20[4][7]=nb20[5][5]=nb20[6][5]=nb20[7][7]=nb20[8][3]=nb20[9][3]=nb20[10][7]=nb20[11][5]=nb20[12][1]=nb20[13][1]=nb20[14][1]=nb20[15][1]=nb20[16][1]=nb20[17][1]=nb20[18][7]=nb20[19][5]=nb20[20][7]=nb20[21][5]=nb20[22][3]=nb20[23][3]=nb20[0][5];
	nb20[1][4]=nb20[2][6]=nb20[3][4]=nb20[4][2]=nb20[5][2]=nb20[6][6]=nb20[7][4]=nb20[8][6]=nb20[9][4]=nb20[10][2]=nb20[11][2]=nb20[12][6]=nb20[13][4]=nb20[14][6]=nb20[15][4]=nb20[16][2]=nb20[17][2]=nb20[18][0]=nb20[19][0]=nb20[20][0]=nb20[21][0]=nb20[22][0]=nb20[23][0]=nb20[0][6];
	nb20[1][5]=nb20[2][7]=nb20[3][5]=nb20[4][3]=nb20[5][3]=nb20[6][7]=nb20[7][5]=nb20[8][7]=nb20[9][5]=nb20[10][3]=nb20[11][3]=nb20[12][7]=nb20[13][5]=nb20[14][7]=nb20[15][5]=nb20[16][3]=nb20[17][3]=nb20[18][1]=nb20[19][1]=nb20[20][1]=nb20[21][1]=nb20[22][1]=nb20[23][1]=nb20[0][7];
	nb20[1][8]=nb20[2][8]=nb20[3][8]=nb20[4][8]=nb20[5][8]=nb20[6][8]=nb20[7][8]=nb20[8][8]=nb20[9][8]=nb20[10][8]=nb20[11][8]=nb20[12][8]=nb20[13][8]=nb20[14][8]=nb20[15][8]=nb20[16][8]=nb20[17][8]=nb20[18][8]=nb20[19][8]=nb20[20][8]=nb20[21][8]=nb20[22][8]=nb20[23][8]=nb20[0][8];
	nb20[1][9]=nb20[2][9]=nb20[3][9]=nb20[4][9]=nb20[5][9]=nb20[6][9]=nb20[7][9]=nb20[8][9]=nb20[9][9]=nb20[10][9]=nb20[11][9]=nb20[12][9]=nb20[13][9]=nb20[14][9]=nb20[15][9]=nb20[16][9]=nb20[17][9]=nb20[18][9]=nb20[19][9]=nb20[20][9]=nb20[21][9]=nb20[22][9]=nb20[23][9]=nb20[0][9];
	
	for (i=0; i<10; i++) 
	{
		
		switch (nb20[1][i]/4)
	{case 0 : break;
	 case 1 : nb20[1][i]=nb20[1][i]+4; break;
	 case 2 : nb20[1][i]=nb20[1][i]-4; break;
	 case 3 : break;
	 case 4 : break;}

		switch (nb20[2][i]/4)
	{case 0 : break;
	 case 1 : break;
	 case 2 : nb20[2][i]=nb20[2][i]+4; break;
	 case 3 : nb20[2][i]=nb20[2][i]-4; break;
	 case 4 : break;}
	
		switch (nb20[3][i]/4)
	{case 0 : break;
	 case 1 : nb20[3][i]=nb20[3][i]+4; break;
	 case 2 : nb20[3][i]=nb20[3][i]+4; break;
	 case 3 : nb20[3][i]=nb20[3][i]-8; break;
	 case 4 : break;}

		switch (nb20[4][i]/4)
	{case 0 : break;
	 case 1 : nb20[4][i]=nb20[4][i]+8; break;
	 case 2 : nb20[4][i]=nb20[4][i]-4; break;
	 case 3 : nb20[4][i]=nb20[4][i]-4; break;
	 case 4 : break;}

		switch (nb20[5][i]/4)
	{case 0 : break;
	 case 1 : nb20[5][i]=nb20[5][i]+8; break;
	 case 2 : break;
	 case 3 : nb20[5][i]=nb20[5][i]-8; break;
	 case 4 : break;}

		switch (nb20[6][i]/4)
	{case 0 : break;
	 case 1 : break;
	 case 2 : break;
	 case 3 : nb20[6][i]=nb20[6][i]+4; break;
	 case 4 : nb20[6][i]=nb20[6][i]-4; break;}

		switch (nb20[7][i]/4)
	{case 0 : break;
	 case 1 : nb20[7][i]=nb20[7][i]+4; break;
	 case 2 : nb20[7][i]=nb20[7][i]-4; break;
	 case 3 : nb20[7][i]=nb20[7][i]+4; break;
	 case 4 : nb20[7][i]=nb20[7][i]-4; break;}

		switch (nb20[8][i]/4)
	{case 0 : break;
	 case 1 : break;
	 case 2 : nb20[8][i]=nb20[8][i]+4; break;
	 case 3 : nb20[8][i]=nb20[8][i]+4; break;
	 case 4 : nb20[8][i]=nb20[8][i]-8; break;}

		switch (nb20[9][i]/4)
	{case 0 : break;
	 case 1 : nb20[9][i]=nb20[9][i]+4; break;
	 case 2 : nb20[9][i]=nb20[9][i]+4; break;
	 case 3 : nb20[9][i]=nb20[9][i]+4; break;
     case 4 : nb20[9][i]=nb20[9][i]-12; break;}

		switch (nb20[10][i]/4)
	{case 0 : break;
	 case 1 : nb20[10][i]=nb20[10][i]+8; break;
	 case 2 : nb20[10][i]=nb20[10][i]-4; break;
	 case 3 : nb20[10][i]=nb20[10][i]+4; break;
	 case 4 : nb20[10][i]=nb20[10][i]-8; break;}

		switch (nb20[11][i]/4)
	{case 0 : break;
	 case 1 : nb20[11][i]=nb20[11][i]+8; break;
	 case 2 : break;
	 case 3 : nb20[11][i]=nb20[11][i]+4; break;
	 case 4 : nb20[11][i]=nb20[11][i]-12; break;}

		switch (nb20[12][i]/4)
	{case 0 : break;
	 case 1 : break;
	 case 2 : nb20[12][i]=nb20[12][i]+8; break;
	 case 3 : nb20[12][i]=nb20[12][i]-4; break;
	 case 4 : nb20[12][i]=nb20[12][i]-4; break;}
	
		switch (nb20[13][i]/4)
	{case 0 : break;
	 case 1 : nb20[13][i]=nb20[13][i]+4; break;
	 case 2 : nb20[13][i]=nb20[13][i]+8; break;
	 case 3 : nb20[13][i]=nb20[13][i]-8; break;
	 case 4 : nb20[13][i]=nb20[13][i]-4; break;}

		switch (nb20[14][i]/4)
	{case 0 : break;
	 case 1 : break;
	 case 2 : nb20[14][i]=nb20[14][i]+8; break;
	 case 3 : break;
	 case 4 : nb20[14][i]=nb20[14][i]-8; break;}

		switch (nb20[15][i]/4)
	{case 0 : break;
	 case 1 : nb20[15][i]=nb20[15][i]+4; break;
	 case 2 : nb20[15][i]=nb20[15][i]+8; break;
	 case 3 : break;
	 case 4 : nb20[15][i]=nb20[15][i]-12; break;}
	
		switch (nb20[16][i]/4)
	{case 0 : break;
	 case 1 : nb20[16][i]=nb20[16][i]+8; break;
	 case 2 : nb20[16][i]=nb20[16][i]+8; break;
	 case 3 : nb20[16][i]=nb20[16][i]-8; break;
	 case 4 : nb20[16][i]=nb20[16][i]-8; break;}

		switch (nb20[17][i]/4)
	{case 0 : break;
	 case 1 : nb20[17][i]=nb20[17][i]+8; break;
	 case 2 : nb20[17][i]=nb20[17][i]+8; break;
	 case 3 : nb20[17][i]=nb20[17][i]-4; break;
	 case 4 : nb20[17][i]=nb20[17][i]-12; break;}

		switch (nb20[18][i]/4)
	{case 0 : break;
	 case 1 : nb20[18][i]=nb20[18][i]+12; break;
	 case 2 : nb20[18][i]=nb20[18][i]-4; break;
	 case 3 : nb20[18][i]=nb20[18][i]-4; break;
	 case 4 : nb20[18][i]=nb20[18][i]-4; break;}
	
		switch (nb20[19][i]/4)
	{case 0 : break;
	 case 1 : nb20[19][i]=nb20[19][i]+12; break;
	 case 2 : break;
	 case 3 : nb20[19][i]=nb20[19][i]-8; break;
	 case 4 : nb20[19][i]=nb20[19][i]-4; break;}

		switch (nb20[20][i]/4)
	{case 0 : break;
	 case 1 : nb20[20][i]=nb20[20][i]+12; break;
	 case 2 : nb20[20][i]=nb20[20][i]-4; break;
	 case 3 : break;
	 case 4 : nb20[20][i]=nb20[20][i]-8; break;}

		switch (nb20[21][i]/4)
	{case 0 : break;
	 case 1 : nb20[21][i]=nb20[21][i]+12; break;
	 case 2 : break;
	 case 3 : break;
	 case 4 : nb20[21][i]=nb20[21][i]-12; break;}
	
		switch (nb20[22][i]/4)
	{case 0 : break;
	 case 1 : nb20[22][i]=nb20[22][i]+12; break;
	 case 2 : nb20[22][i]=nb20[22][i]+4; break;
	 case 3 : nb20[22][i]=nb20[22][i]-8; break;
	 case 4 : nb20[22][i]=nb20[22][i]-8; break;}

		switch (nb20[23][i]/4)
	{case 0 : break;
	 case 1 : nb20[23][i]=nb20[23][i]+12; break;
	 case 2 : nb20[23][i]=nb20[23][i]+4; break;
	 case 3 : nb20[23][i]=nb20[23][i]-4; break;
	 case 4 : nb20[23][i]=nb20[23][i]-12; break;}

	
	}
	
	k=1;
	for (j=1; j<24; j++) {nx=0;for (i=0; i<10; i++) {nx=20*nx + nb20[j][i];}   if (nx<min) min=nx; if (nx==nb) k++;}
	return1=24/k; if (nb==min) return2=true; else return2=false;  /*Renvoie return1 et return2 au main; return1 est la multiplicit� (1, 2, 3, 4, 6, 12 ou 24)
														  et return2 vaut 0 (skipper le calcul) ou 1 (faire le calcul complet)*/
}

/*Ce programme calcule les machines � 5 �tats. La bande est limit�e � 100 cases. Les (nouvelles, par rapport � S<5) MT qui se sont arr�t�es sont imprim�es dans test. 
Celles qui travaillent toujours apr�s 2500 pas sont not�es dans nest. Celles qui ont d�bord� sont rejet�es dans overtape*/

int main()
{long long int i,quo, nb, nx, halt=0, loop=0, nest=0, dejavus[1917]={2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 
21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 
38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 
55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 
72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 
89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 
105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 
119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 
133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 
147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 
161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 
175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 
189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 
203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215, 216, 
217, 218, 219, 220, 221, 222, 223, 224, 225, 226, 227, 228, 229, 230, 
231, 232, 233, 234, 235, 236, 237, 238, 239, 240, 241, 242, 243, 244, 
245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255, 256, 257, 258, 
259, 260, 261, 262, 263, 264, 265, 266, 267, 268, 269, 270, 271, 272, 
273, 275, 276, 277, 278, 280, 281, 282, 283, 284, 285, 286, 287, 288, 
289, 290, 291, 292, 293, 294, 297, 298, 299, 300, 301, 302, 303, 304, 
305, 307, 309, 310, 311, 313, 315, 316, 318, 319, 320, 321, 322, 323, 
324, 325, 328, 329, 330, 331, 332, 333, 334, 335, 336, 337, 338, 339, 
340, 341, 342, 343, 344, 345, 346, 347, 348, 349, 350, 351, 352, 353, 
355, 356, 357, 358, 359, 361, 362, 363, 364, 365, 366, 367, 368, 370, 
371, 373, 374, 375, 376, 377, 378, 379, 380, 381, 382, 383, 384, 385, 
386, 387, 388, 389, 390, 391, 392, 393, 394, 395, 396, 397, 398, 399, 
400, 401, 402, 403, 404, 405, 406, 407, 408, 409, 410, 411, 412, 413, 
414, 415, 416, 417, 418, 419, 420, 421, 422, 423, 424, 425, 426, 427, 
428, 429, 430, 431, 432, 433, 434, 435, 436, 437, 438, 439, 440, 441, 
442, 443, 444, 445, 446, 447, 448, 449, 450, 451, 452, 453, 454, 455, 
456, 457, 458, 459, 460, 461, 462, 463, 464, 465, 466, 467, 468, 469, 
470, 471, 472, 473, 474, 475, 476, 477, 478, 479, 480, 481, 482, 483, 
484, 485, 486, 487, 488, 489, 490, 491, 492, 493, 494, 495, 496, 497, 
498, 499, 500, 501, 502, 503, 504, 505, 506, 507, 508, 509, 510, 511,
512, 513, 514, 515, 517, 520, 523, 526, 527, 528, 529, 530, 532, 536, 
538, 539, 540, 542, 543, 544, 549, 570, 571, 574, 576, 579, 581, 585, 
586, 587, 597, 611, 615, 619, 621, 623, 629, 636, 638, 639, 640, 641, 
648, 650, 651, 656, 657, 658, 659, 660, 661, 663, 673, 674, 676, 677, 
680, 681, 682, 683, 685, 686, 687, 693, 694, 698, 699, 702, 703, 705, 
707, 713, 716, 717, 718, 725, 726, 727, 729, 730, 731, 733, 734, 735, 
736, 746, 747, 749, 750, 751, 755, 758, 759, 760, 762, 764, 765, 766, 
767, 768, 769, 770, 771, 772, 773, 774, 775, 776, 777, 778, 779, 781, 
783, 784, 785, 786, 787, 788, 789, 790, 793, 795, 796, 800, 801, 802, 
803, 804, 805, 806, 807, 808, 809, 810, 811, 812, 813, 814, 815, 816, 
817, 818, 819, 820, 821, 822, 823, 824, 825, 827, 828, 829, 830, 831, 
832, 833, 834, 835, 836, 837, 840, 841, 842, 843, 845, 846, 848, 849, 
850, 851, 852, 853, 854, 855, 856, 857, 858, 859, 860, 861, 862, 863, 
864, 865, 866, 867, 868, 869, 870, 871, 872, 873, 874, 875, 876, 877, 
878, 879, 881, 883, 884, 885, 886, 887, 890, 891, 892, 893, 894, 895, 
896, 897, 898, 899, 900, 901, 902, 903, 904, 905, 906, 907, 908, 909, 
910, 911, 912, 913, 914, 915, 917, 918, 920, 921, 922, 923, 924, 925, 
926, 927, 928, 929, 930, 931, 932, 933, 935, 936, 937, 938, 939, 940, 
941, 942, 943, 944, 945, 946, 947, 948, 949, 950, 951, 952, 953, 954, 
955, 956, 957, 958, 959, 960, 961, 962, 963, 967, 968, 969, 970, 971, 
972, 973, 974, 975, 976, 978, 979, 980, 981, 982, 983, 984, 985, 986, 
987, 988, 989, 990, 991, 992, 993, 995, 996, 997, 998, 999, 1001, 
1002, 1003, 1004, 1005, 1006, 1007, 1008, 1009, 1011, 1012, 1013, 
1014, 1015, 1016, 1017, 1018, 1019, 1020, 1021, 1022, 1023, 1024, 
1032, 1039, 1056, 1066, 1087, 1150, 1170, 1182, 1183, 1237, 1243, 
1280, 1297, 1313, 1316, 1317, 1322, 1323, 1330, 1345, 1352, 1353, 
1362, 1365, 1369, 1389, 1397, 1399, 1403, 1413, 1427, 1435, 1453, 
1455, 1457, 1462, 1463, 1469, 1471, 1479, 1495, 1511, 1514, 1516, 
1521, 1525, 1526, 1531, 1534, 1535, 1536, 1537, 1538, 1539, 1540, 
1541, 1542, 1543, 1544, 1545, 1546, 1547, 1548, 1551, 1552, 1553, 
1554, 1557, 1561, 1563, 1567, 1568, 1569, 1570, 1572, 1573, 1576, 
1578, 1584, 1585, 1590, 1591, 1593, 1594, 1596, 1600, 1601, 1602, 
1603, 1604, 1605, 1607, 1608, 1609, 1610, 1611, 1612, 1615, 1618, 
1619, 1620, 1621, 1625, 1632, 1633, 1635, 1636, 1638, 1642, 1645,
1649, 1650, 1651, 1656, 1660, 1661, 1662, 1663, 1664, 1665, 1666, 
1667, 1670, 1672, 1673, 1674, 1676, 1681, 1682, 1683, 1684, 1685, 
1689, 1690, 1691, 1693, 1696, 1698, 1699, 1701, 1704, 1705, 1706, 
1707, 1709, 1711, 1716, 1717, 1718, 1719, 1722, 1723, 1724, 1725, 
1726, 1727, 1728, 1729, 1733, 1738, 1741, 1743, 1744, 1746, 1748, 
1749, 1750, 1751, 1753, 1754, 1755, 1757, 1758, 1759, 1765, 1770, 
1771, 1772, 1773, 1774, 1775, 1779, 1781, 1782, 1783, 1786, 1787, 
1788, 1789, 1790, 1791, 1792, 1793, 1795, 1796, 1798, 1799, 1800, 
1802, 1804, 1805, 1806, 1808, 1810, 1812, 1813, 1816, 1817, 1818, 
1819, 1824, 1826, 1828, 1829, 1831, 1833, 1834, 1836, 1837, 1840, 
1841, 1842, 1843, 1846, 1847, 1849, 1851, 1852, 1854, 1855, 1856, 
1857, 1859, 1861, 1865, 1866, 1869, 1873, 1874, 1875, 1876, 1877, 
1878, 1879, 1882, 1883, 1885, 1886, 1887, 1888, 1891, 1892, 1894, 
1896, 1898, 1899, 1900, 1901, 1902, 1903, 1907, 1909, 1910, 1911, 
1912, 1913, 1914, 1915, 1917, 1918, 1919, 1920, 1921, 1923, 1926,
1927, 1930, 1935, 1938, 1939, 1945, 1947, 1949, 1950, 1951, 1953, 
1958, 1962, 1963, 1965, 1966, 1967, 1971, 1973, 1974, 1975, 1976, 
1977, 1978, 1979, 1981, 1982, 1983, 1984, 1985, 1987, 1989, 1990, 
1991, 1993, 1996, 1997, 1998, 1999, 2005, 2007, 2008, 2009, 2010, 
2011, 2013, 2014, 2015, 2016, 2017, 2020, 2025, 2027, 2028, 2029, 
2030, 2031, 2032, 2034, 2035, 2037, 2038, 2039, 2040, 2041, 2042, 
2043, 2044, 2045, 2046, 2047, 2048, 2075, 2111, 2176, 2267, 2303, 
2358, 2400, 2454, 2459, 2487, 2633, 2635, 2642, 2644, 2698, 2724, 
2730, 2731, 2778, 2779, 2794, 2811, 2867, 2903, 2907, 2924, 2925, 
3003, 3035, 3070, 3071, 3072, 3073, 3076, 3077, 3080, 3087, 3088, 
3089, 3103, 3104, 3114, 3115, 3136, 3137, 3142, 3154, 3157, 3170, 
3172, 3193, 3198, 3200, 3201, 3204, 3206, 3208, 3209, 3210, 3217, 
3218, 3219, 3221, 3224, 3230, 3236, 3243, 3265, 3274, 3289, 3296, 
3301, 3321, 3327, 3345, 3348, 3349, 3353, 3363, 3364, 3371, 3381, 
3392, 3394, 3400, 3411, 3412, 3413, 3414, 3415, 3434, 3435, 3437, 
3445, 3446, 3469, 3483, 3489, 3492, 3494, 3498, 3499, 3500, 3501, 
3503, 3507, 3508, 3509, 3510, 3511, 3515, 3518, 3519, 3543, 3547, 
3548, 3549, 3550, 3551, 3559, 3562, 3567, 3574, 3575, 3579, 3581, 
3583, 3584, 3585, 3589, 3590, 3592, 3601, 3602, 3609, 3618, 3626, 
3627, 3632, 3639, 3643, 3644, 3647, 3648, 3656, 3657, 3659, 3660, 
3661, 3673, 3678, 3680, 3684, 3686, 3687, 3693, 3711, 3712, 3715, 
3720, 3732, 3739, 3745, 3746, 3753, 3754, 3755, 3757, 3758, 3764, 
3765, 3767, 3770, 3773, 3774, 3775, 3776, 3779, 3782, 3784, 3785, 
3788, 3790, 3791, 3797, 3798, 3799, 3800, 3801, 3802, 3803, 3804, 
3805, 3806, 3807, 3819, 3822, 3823, 3829, 3830, 3831, 3834, 3835, 
3837, 3838, 3839, 3840, 3841, 3847, 3848, 3855, 3865, 3867, 3868, 
3871, 3873, 3876, 3877, 3891, 3897, 3899, 3903, 3925, 3930, 3933, 
3934, 3939, 3943, 3945, 3947, 3949, 3951, 3952, 3953, 3956, 3957, 
3959, 3961, 3967, 3968, 3969, 3976, 3989, 3992, 3995, 4010, 4011, 
4021, 4023, 4024, 4026, 4027, 4029, 4030, 4031, 4032, 4035, 4053, 
4061, 4062, 4063, 4064, 4067, 4068, 4076, 4077, 4078, 4079, 4081, 
4086, 4087, 4089, 4090, 4091, 4092, 4093, 4094, 4095, 4681, 5120, 
5189, 5266, 5337, 5461, 5563, 5815, 5851, 6007, 6143, 6144, 6145, 
6157, 6177, 6276, 6291, 6324, 6333, 6416, 6419, 6421, 6429, 6436, 
6437, 6473, 6474, 6482, 6489, 6540, 6553, 6650, 6655, 6656, 6657,
6666, 6675, 6692, 6728, 6729, 6743, 6765, 6821, 6822, 6824, 6825, 
6826, 6827, 6830, 6831, 6837, 6890, 6911, 6918, 6981, 6997, 6998, 
7002, 7019, 7021, 7102, 7119, 7131, 7134, 7166, 7167, 7168, 7169, 
7170, 7192, 7231, 7240, 7264, 7277, 7291, 7332, 7361, 7372, 7373, 
7508, 7509, 7515, 7517, 7530, 7533, 7541, 7551, 7552, 7568, 7579, 
7597, 7606, 7607, 7611, 7613, 7637, 7643, 7645, 7646, 7660, 7671, 
7677, 7679, 7680, 7681, 7745, 7756, 7783, 7794, 7807, 7851, 7867, 
7870, 7871, 7894, 7898, 7899, 7901, 7915, 7918, 7919, 7923, 7927, 
7930, 7935, 7999, 8032, 8042, 8045, 8052, 8055, 8058, 8059, 8061, 
8062, 8063, 8092, 8119, 8123, 8126, 8127, 8149, 8153, 8171, 8175, 
8178, 8182, 8185, 8188, 8189, 8190, 8191, 8448, 9216, 10307, 10497, 
10911, 10917, 10922, 10923, 11606, 11693, 11702, 12287, 12288, 12289, 
12353, 12565, 12672, 12799, 12801, 12873, 13055, 13056, 13165, 13207, 
13311, 13332, 13471, 13477, 13483, 13493, 13526, 13588, 13610, 13652, 
13653, 13655, 13658, 13675, 13741, 13786, 13806, 13818, 13855, 13962, 
13970, 14005, 14007, 14037, 14042, 14043, 14189, 14279, 14293, 14326, 
14335, 14336, 14337, 14456, 14592, 14593, 14700, 14712, 14744, 14866, 
14920, 14953, 15018, 15019, 15083, 15167, 15194, 15195, 15200, 15213, 
15274, 15326, 15350, 15352, 15358, 15381, 15388, 15391, 15688, 15701, 
15709, 15735, 15743, 15798, 15803, 15935, 15968, 15992, 16094, 16118, 
16119, 16128, 16239, 16255, 16256, 16310, 16335, 16367, 16373, 16375, 
16377, 16380, 16381, 16382, 16383, 16640, 18724, 20479, 20993, 21025, 
21653, 21845, 22189, 24575, 24576, 24577, 24705, 25621, 25746, 25756,
25758, 25759, 25898, 25899, 26037, 26709, 27274, 27306, 27307, 27309, 
27350, 27476, 27508, 27643, 27989, 28011, 28030, 28085, 28086, 28398, 
28671, 29596, 30037, 30125, 30203, 30389, 30426, 30427, 30583, 30653, 
30719, 31039, 31182, 31397, 31661, 31675, 31735, 31744, 32239, 32375, 
32512, 32760, 32764, 32765, 32766, 32767, 49152, 49153, 49155, 49704, 
51492, 53313, 54612, 54613, 54615, 56173, 56662, 56799, 57197, 57207, 
57341, 57343, 58569, 58982, 59802, 60074, 60539, 60855, 61151, 62263, 
63351, 63357, 63479, 63487, 64171, 64365, 64989, 65397, 65533, 65534, 
65535, 87381, 93622, 103765, 106098, 109226, 109227, 109237, 109270, 
112347, 114687, 117991, 118136, 120149, 120171, 120501, 120661, 
121240, 121708, 121709, 122333, 122782, 126520, 126702, 126805, 
126843, 128378, 130031, 130301, 130681, 131070, 131071, 174762, 
187243, 209697, 218453, 224617, 224674, 224694, 240298, 240299, 
240341, 241002, 241784, 243419, 244458, 245246, 253695, 257144, 
257915, 262127, 262141, 262142, 262143, 349525, 436906, 436907, 
449450, 486838, 489334, 524286, 524287, 786433, 823881, 873813, 
884734, 898779, 917503, 961883, 972202, 973677, 1048572, 1048574, 
1048575, 1657124, 1747626, 1947355, 2097151, 3295524, 3495253, 
3495255, 3670013, 3670015, 4194303, 5592405, 5991862, 6990506, 
8363209, 8388607, 13981013, 15641258, 16777215, 27962026, 33554430, 
33554431, 55924053, 64872319, 67108863, 111848106, 131986815, 
134217726, 134217727, 223696213, 249261494, 268435454, 268435455, 
536870911, 894784852, 894784853, 894784855, 1073741821, 1073741823/*, 
3579139413, 4294967295, 8589934591, 17179869183, 57266230613, 
64424509293, 68719476735, 137438953471, 251971414699, 549755813887, 
1099511627775, 3665038759253, 4398046511103, 8796093022207, 
17592186044415, 35184372088831, 65342405307830, 234562480592213, 
241264265751990*/};   //Indices des suites imprim�es jusqu'� l'ordre 4 inclus
int j=0,j2=0,k,k1,k2,kk,ipas,jj=0,j1,nbfig[2*5+1],state,startstate,starttape[201],pos,u,posmax,posmin,posleft,posright,tripl,tape[201],return1,coupure[7]={4,16,48,96,300,1000,2500}; 
bool return2, ok;
std::vector<unsigned char> res(2147483648, '0');   //Stocke les suites imprim�es jusqu'� 30 bits (2147483648=2^31)
for (k2 = 0; k2<1936;k2++) res[dejavus[k2]]='1';     //Chargement dans res des (entiers correspondants aux) suites d�j� imprim�es jusqu'� l'ordre 4 inclus

 ofstream fichier("halting1.txt", ios::out | ios::trunc);  //ouverture en �criture avec effacement du fichier ouvert
 ofstream remise("nested1.txt", ios::out | ios::trunc);
 ofstream overtape("overtape1.txt", ios::out | ios::trunc);

for (nb = 202400000000; nb <802400000000/*00*/; nb+=16) { 

	sym5(nb,return1,return2); if (return2 == false)  continue;   //on ignore les machines sym�triques (on passe au nb suivant)

	else {quo=nb; for (i=10; i>0; i--) {nbfig[i] = quo%20; quo=quo/20;}   //d�composition de nb en base 4S


	for (k1=0;k1<201;k1++) tape[k1]=0; pos=1; state=tripl=0; ipas=1; posmax=1;    //initialisation de la machine nb


	for (k=0; k<6;k++)   {posmin=pos;posright=pos;posleft=posmax;startstate=state; for (kk=posleft; kk>0;kk--) {starttape[kk]=tape[kk];}    
						 //on explore son �volution par fen�tres successives de longueurs coupure[k]; attention que k<6 signifie qu'on s'arr�te � la 6�me fen�tre



while (pos>0  && ipas<coupure[k]) { tripl=nbfig[10-2*state-tape[pos]]; state=tripl/4; tape[pos]=(tripl/2) % 2; pos=pos+1-2*(tripl % 2);
if (pos>200) {overtape <<nb  <<" , "; if (j2%10==9) overtape <<"\n"; j2++;goto cpt;}    //on a d�bord� d'une bande � 100 cases
       if (pos>posmax) posmax=pos; if (pos<posmin) posmin=pos; 
	   ok=true; if ((state != startstate) || ((posleft-posright) != (posmax-pos))) {ok=false; goto next;}

	   for (kk=0;kk<posleft-posmin+1;kk++) { if (tape[posmax-kk]!=starttape[posleft-kk]) {ok=false; goto next;};} 
								   if (ok==true) {loop=loop+return1; goto suivante;}
                   next : ipas++; }
 /*while est termin� : soit pos=0 (=halt) soit ipas a atteint le bas de la fen�tre sans trouver de p�riodicit� (= on passe � la fen�tre suivante)*/


								   if (pos==0) {halt=halt+return1; if (posmax<31) {nx=1;for (i=posmax; i>0; i--) {nx=2*nx + tape[i];} ;}
             if ((posmax<31 && res[nx]=='0')||posmax>30)  { 
			 fichier <<"{ {" ;   for (i=posmax; i>1; i--) {fichier << tape[i]    <<" , ";}   fichier << tape[1] <<" }, "  << nb  <<" , " << ipas/2  <<" }, "  <<"\n";  res[nx]='1';}    goto suivante;}  
			 /*Le if int�rieur regarde si la suite cod�e par nx est imprim�e pour la premi�re fois. 
			 nx est l'entier binaire obtenu en pr�fixant par 1 la suite calcul�e.*/  	

				
					    ;}  //fin du for pr�c�dent le while


		;}   //fin du else


suivante : if (ok==false && pos != 0) {nest=nest+return1; remise <<nb <<"," ; if (j%10==9) remise <<"\n"; j++;} //on comptabilise dans nest les machines qui ne sont ni dans halt ni dans loop

     cpt:;}  //fin du for nb

fichier << "Nombre de machines qui se sont certainement arr�t�es : "  << halt  <<"\n";
fichier << "Nombre de machines qui ont certainement boucl� : "  << loop  <<"\n";
fichier << "Nombre de machines qui s'arr�teront ou qui boucleront ou qui ont nest� : "  << nest  <<"\n";
fichier.close(); remise.close(); overtape.close();
return(0)                               
	
;}







 /*Lorsque S=5, il est possible que le container res ne suffise plus. On peut imaginer pr�voir un fichier d'exceptions plus longues qu'un certain seuil.
long long exceptions[1000]={0}; permet de stocker un maximum de 1000 exceptions consommant 13 bits et plus
if (nx>4095) {except=false; for(j1=0;j1<jj;j1++) if(nx==exceptions[j1]) except=true; 
if (except==false)   {j1++; resu[j]=nx; fichier << nb  <<" : " << resu[j] <<"\n"; exceptions[j1]=nx;};}

if ((nx<4096) && (res[nx]==false))  {j++; resu[j]=nx; fichier << nb  <<" : " << resu[j] <<"\n";  res[nx]=true;}*/


/*
 

        ofstream fichier("test.txt", ios::out | ios::trunc);  // ouverture en �criture avec effacement du fichier ouvert
 
        if(fichier)
        {
                string nom = "Xav57";
                int age = 19;
                fichier << "Date de naissance : " << 24 << '/' << 3 << '/' << 1988 << endl;
                fichier << "Bonjour, " << nom << ". Vous avez " << age << " ans.";
 
                fichier.close();
        }
        else
                cerr << "Impossible d'ouvrir le fichier !" << endl;
 
        */
  
/*int main()
{

    std::hash_map <std::vector<int>, std::pair<long long, int>, MyClass_Hasher >  map;

	long long i,limit=4294967296;

    FILE* f = fopen("MT4Cpu.txt", "w");
    for (i = 1000200; i < limit; i+=2) {if (sym4(i) % 2 == 0) continue;
	    Machine mTest(i);
        mTest.process(f, map);  
    }



    std::vector<std::pair<long long, std::pair<int, std::vector<int> > > > allMachines;

    {
        std::hash_map<std::vector<int>, std::pair<long long, int>, MyClass_Hasher >::const_iterator iter = map.begin();
        std::hash_map<std::vector<int>, std::pair<long long, int>, MyClass_Hasher >::const_iterator end = map.end();
        for (; iter != end; ++iter) {
            allMachines.push_back(std::make_pair(iter->second.first, std::make_pair(iter->second.second, iter->first)));
        }
    }

    std::sort(allMachines.begin(), allMachines.end());

    {
        std::vector<std::pair<long long, std::pair<int, std::vector<int> > > >::const_iterator iter = allMachines.begin();
        std::vector<std::pair<long long, std::pair<int, std::vector<int> > > >::const_iterator end  = allMachines.end();

		fprintf(f, "Nombre de MT ex�cut�es : %lld\n",limit);
		fprintf(f, "Nombre de MT paires qui se sont arr�t�es : %lld\n",halt); 

        for (; iter != end; ++iter) {
            const std::vector<int>& vect = (iter->second.second);
            fprintf(f, "{%lld, %d, {", iter->first, iter->second.first);
			

            if (vect.size() >= 0) {
                std::vector<int>::const_reverse_iterator tapeIter = vect.rbegin();
                std::vector<int>::const_reverse_iterator tapeEnd = vect.rend();
                for (; tapeIter != tapeEnd-1; ++tapeIter) {
                    fprintf(f, "%d, ", *tapeIter);
                }
                fprintf(f, "%d", *tapeIter);
            }

            fprintf(f, "}}\n");
        }        
    }

    fclose(f);
    std::cout << "process terminated" << std::endl;
    std::cin;
}*/