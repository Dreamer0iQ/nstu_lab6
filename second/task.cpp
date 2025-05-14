#include <iostream>
#include <vector>
#include <array>
#include <cstdint>
#include <iomanip>
#include <fstream>
#include <random>

using namespace std;

using State = array<array<uint8_t, 4>, 4>; //хранения состояния AES — матрицы 4×4 байт
using Key = array<uint8_t, 16>;

using RoundKeys = array<Key, 11>;

const int Nk = 4; 
const int Nb = 4; 
const int Nr = 10; // number of rounds

const uint8_t rcon[11] = {
    0x00,
    0x01, 0x02, 0x04, 0x08,
    0x10, 0x20, 0x40, 0x80,
    0x1B, 0x36
};

// Простой S-box (укороченный для примера)
const uint8_t sbox[256] = {
    0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5,
    0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
    0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0,
    0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
    0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc,
    0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
    0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a,
    0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
    0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0,
    0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
    0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b,
    0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
    0xd0, 0xef, 0xaa, 0xfd, 0x43, 0x4d, 0x33, 0x85,
    0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
    0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5,
    0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
    0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17,
    0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
    0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88,
    0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
    0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c,
    0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
    0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9,
    0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
    0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6,
    0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
    0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e,
    0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
    0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94,
    0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
    0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68,
    0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16
};

uint8_t getSBoxValue(uint8_t num) {
    return sbox[num];
}

RoundKeys expandKey(const Key& key) {
    RoundKeys roundKeys;

    uint8_t expanded[176]; // 11 * 16
    int i = 0;
    while (i < Nk) {
        expanded[i * 4 + 0] = key[i * 4 + 0];
        expanded[i * 4 + 1] = key[i * 4 + 1];
        expanded[i * 4 + 2] = key[i * 4 + 2];
        expanded[i * 4 + 3] = key[i * 4 + 3];
        ++i;
    }

    uint8_t temp[4];
    while (i < Nb * (Nr + 1)) {
        for (int j = 0; j < 4; ++j)
            temp[j] = expanded[(i - 1) * 4 + j];

        if (i % Nk == 0) {
            // RotWord
            uint8_t k = temp[0];
            temp[0] = temp[1];
            temp[1] = temp[2];
            temp[2] = temp[3];
            temp[3] = k;

            // SubWord
            for (int j = 0; j < 4; ++j)
                temp[j] = getSBoxValue(temp[j]);

            temp[0] ^= rcon[i / Nk];
        }

        for (int j = 0; j < 4; ++j)
            expanded[i * 4 + j] = expanded[(i - Nk) * 4 + j] ^ temp[j];
        ++i;
    }

    for (int r = 0; r < 11; ++r)
        for (int j = 0; j < 16; ++j)
            roundKeys[r][j] = expanded[r * 16 + j];

    return roundKeys;
}

// XOR состояния с ключом
void addRoundKey(State& state, const Key& roundKey) {
    for (int i = 0; i < 16; ++i) {
        state[i % 4][i / 4] ^= roundKey[i];
    }
}

// SubBytes — байтовая подстановка
void subBytes(State& state) {
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            state[i][j] = sbox[state[i][j]];
}

// ShiftRows — сдвиг строк
void shiftRows(State& state) {
    for (int r = 1; r < 4; ++r) {
        array<uint8_t, 4> tmp;
        for (int c = 0; c < 4; ++c)
            tmp[c] = state[(c + r) % 4][r];
        for (int c = 0; c < 4; ++c)
            state[c][r] = tmp[c];
    }
}

uint8_t xtime(uint8_t x) {
    return (x << 1) ^ ((x >> 7) * 0x1b);
}

void mixSingleColumn(uint8_t* r) {
    uint8_t a[4], b[4];
    for (int i = 0; i < 4; ++i) {
        a[i] = r[i];
        b[i] = xtime(r[i]);
    }
    r[0] = b[0] ^ a[1] ^ b[1] ^ a[2] ^ a[3];
    r[1] = a[0] ^ b[1] ^ a[2] ^ b[2] ^ a[3];
    r[2] = a[0] ^ a[1] ^ b[2] ^ a[3] ^ b[3];
    r[3] = a[0] ^ b[0] ^ a[1] ^ a[2] ^ b[3];
}

void mixColumns(State& state) {
    for (int c = 0; c < 4; ++c) {
        uint8_t col[4];
        for (int i = 0; i < 4; ++i)
            col[i] = state[c][i];
        mixSingleColumn(col);
        for (int i = 0; i < 4; ++i)
            state[c][i] = col[i];
    }
}

// Преобразование массива в state
State toState(const array<uint8_t, 16>& input) {
    State state{};
    for (int i = 0; i < 16; ++i)
        state[i % 4][i / 4] = input[i];
    return state;
}

// Преобразование state обратно в массив
array<uint8_t, 16> fromState(const State& state) {
    array<uint8_t, 16> output{};
    for (int i = 0; i < 16; ++i)
        output[i] = state[i % 4][i / 4];
    return output;
}

// Один раунд AES
void aesRound(State& state, const Key& roundKey) {
    subBytes(state);
    shiftRows(state);
    mixColumns(state);
    addRoundKey(state, roundKey);
}

void aesFinalRound(State& state, const Key& roundKey) {
    subBytes(state);
    shiftRows(state);
    addRoundKey(state, roundKey);
}

vector<uint8_t> aesEncryptCFB(vector<uint8_t> plaintext, const Key& key, const array<uint8_t, 16>& iv) {
    vector<uint8_t> ciphertext;
    array<uint8_t, 16> currentIV = iv;

    RoundKeys roundKeys = expandKey(key);

    for (size_t i = 0; i < plaintext.size(); i += 16) {
        array<uint8_t, 16> block{};
        for (size_t j = 0; j < 16 && i + j < plaintext.size(); ++j){
            block[j] = plaintext[i + j];
        }

        State state = toState(currentIV);
        addRoundKey(state, roundKeys[0]);

        for (int z = 1; z <= 9; ++z){
            aesRound(state, roundKeys[z]);
        }
        aesFinalRound(state, roundKeys[10]);
        
        auto encryptedIV = fromState(state);            

        for (int j = 0; j < 16 && i + j < plaintext.size(); ++j) {
            uint8_t c = encryptedIV[j] ^ block[j];
            ciphertext.push_back(c);
            currentIV[j] = c;
        }
    }

    return ciphertext;
}

vector<uint8_t> aesDecryptCFB(vector<uint8_t> ciphertext, const Key& key, const array<uint8_t, 16>& iv) {
    vector<uint8_t> plaintext;
    array<uint8_t, 16> currentIV = iv;

    RoundKeys roundKeys = expandKey(key);

    for (size_t i = 0; i < ciphertext.size(); i += 16) {
        array<uint8_t, 16> block{};
        for (size_t j = 0; j < 16 && i + j < ciphertext.size(); ++j)
            block[j] = ciphertext[i + j];

        State state = toState(currentIV);
        addRoundKey(state, roundKeys[0]);

        for (int z = 1; z <= 9; ++z){
            aesRound(state, roundKeys[z]);
        }
        aesFinalRound(state, roundKeys[10]);

        auto encryptedIV = fromState(state);

        for (int j = 0; j < 16 && i + j < ciphertext.size(); ++j) {
            uint8_t p = encryptedIV[j] ^ block[j];
            plaintext.push_back(p);
            currentIV[j] = block[j];
        }
    }

    return plaintext;
}

int main() {
    string text;
    cout << "Введите текст: ";
    getline(cin, text);

    // PKCS#7 Padding (AES работает строго с блоками по 16 байт.)
    size_t pad = 16 - (text.size() % 16);
    for (size_t i = 0; i < pad; ++i)
        text.push_back(static_cast<char>(pad));

    vector<uint8_t> input(text.begin(), text.end());

    // Случайный секретный ключ и вектор инициализации IV
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, 255);

    Key key;
    array<uint8_t, 16> iv;
    for (int i = 0; i < 16; ++i) {
        key[i] = dis(gen);
        iv[i] = dis(gen);
    }

    // Запись ключа в файл
    ofstream keyFile("aes_key.txt");
    for (uint8_t b : key)
        keyFile << hex << setw(2) << setfill('0') << (int)b;
    keyFile.close();

    cout << "Случайный ключ (hex): ";
    for (uint8_t b : key)
        cout << hex << setw(2) << setfill('0') << (int)b << " ";
    cout << "\nВектор инициализации (IV): ";
    for (uint8_t b : iv)
        cout << hex << setw(2) << setfill('0') << (int)b << " ";
    cout << endl;

    auto encrypted = aesEncryptCFB(input, key, iv);

    cout << "Зашифрованный текст (hex): ";
    for (uint8_t b : encrypted)
        cout << hex << setw(2) << setfill('0') << (int)b << " ";
    cout << endl;

    auto decrypted = aesDecryptCFB(encrypted, key, iv);

    // Удаление Padding при расшифровке
    if (!decrypted.empty()) {
        uint8_t lastByte = decrypted.back();
        if (lastByte <= 16) {
            decrypted.resize(decrypted.size() - lastByte);
        }
    }

    cout << "Расшифрованный текст: ";
    for (uint8_t b : decrypted)
        cout << static_cast<char>(b);
    cout << endl;

    return 0;
}