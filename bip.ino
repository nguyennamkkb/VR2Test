// bipset
//độ ngân 1: 75; 2:150; 3:350; 4:500

/* coi thong bao
1. chạm thẻ
2. nhấc thẻ
3. chạm vân tay
4. Thành công
5. lỗi.
#define Cham_the 1
#define Nhac_the 2
#define Cham_van_tay 3
#define Thanh_Cong 4
#define Loi 5
*/
void setbip(int n)
{
    switch (n)
    {
    case Cham_the:
        bip(1, 2);
        break;
    case Nhac_the:
        bip(2, 1);
        break;
    case Cham_van_tay:
        bip(1, 3);
        break;
    case Thanh_Cong:
        bip(2, 1);
        break;
    case Loi:
        bip(3, 1);
        break;
    }
}

void bip(int len, int type)
{
    //
    for (int i = 0; i < len; i++)
    {
        coi_on;
        delay(75 * type);
        coi_off;
        delay(75 * type);
    }
}