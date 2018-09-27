// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

TBitField::TBitField(int len)
{
	Bitlen = len;
	MemLen = 0;
	if (len >= 0) {
		MemLen = (BitLen + (sizeof (TELEM) * 8 - 1)) / (sizeof (TELEM) * 8);
		pMem = new TELEM[MemLen];
		for (int i = 0; i < MemLen; i++) pMem[i] = 0;
	} 
	else throw 0;
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
	BitLen = bf.BitLen;
	MemLen = bf.MemLen;
	pMem = new TELEM[MemLen];
	for (int i = 0; i < MemLen; i++) pMem[i] = bf.pMem[i];
}

TBitField::~TBitField()
{
	delete []pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
	if (n >= 0) {
		return (n / (sizeof(TELEM) * 8));
	}
	else throw 1; 
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
	return (1 << (n - GetMemIndex(n) * sizeof(TELEM) * 8));
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
	return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
	if (n < BitLen && n >= 0)
		pMem[this->GetMemIndex(n)] = pMem[this->GetMemIndex(n)] | this->GetMemMask(n);
	else
		throw 2;
}

void TBitField::ClrBit(const int n) // очистить бит
{
	if (n < BitLen)
		pMem[this->GetMemIndex(n)] = pMem[this->GetMemIndex(n)] & ~this->GetMemMask(n);
	else
		throw 3;
}

int TBitField::GetBit(const int n) const // получить значение бита
{
	if (n >= BitLen) throw 4;
	return (pMem[GetMemIndex(n)] & GetMemMask(n));
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
	if (BitLen != bf.BitLen) {
		MemLen = bf.MemLen;
		BitLen = bf.BitLen;
		delete[] pMem;
		pMem = new TELEM[MemLen];
	}
	for (int i = 0; i < MemLen; i++)
		pMem[i] = bf.pMem[i];
	return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
	if (BitLen == bf.BitLen) {
		for (int i = 0; i < MemLen; i++) {
			if (pMem[i] != bf.pMem[i]) return 0;
		}
	}
	else 
		return 0;
	return 1;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
	if (BitLen == bf.BitLen) {
		for (int i = 0; i < MemLen; i++) {
			if (pMem[i] != bf.pMem[i]) return 1;
		}
	}
	else 
		return 1;
	return 0;
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
	int n;
	TBitField buf(0);
	if (BitLen < bf.BitLen) {
		n = MemLen;
		buf = bf;
	}
	else {
		n = bf.MemLen;
		buf = *this;
	}
	for (int i = 0; i < n; i++) {
		buf.pMem[i] = pMem[i] | bf.pMem[i];
	}
	return buf;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
	int n;
	TBitField buf(0);
	if (BitLen < bf.BitLen) {
		n = MemLen;
		buf = bf;
	}
	else {
		n = bf.MemLen;
		buf = *this;
	}
	for (int i = 0; i < n; i++) {
		buf.pMem[i] = pMem[i] & bf.pMem[i];
	}
	return buf;
}

TBitField TBitField::operator~(void) // отрицание
{
	TBitField tmp = *this;
	for (int i = 0; i < BitLen; i++) {
		if (tmp.GetBit(i))
			tmp.ClrBit(i);
		else
			tmp.SetBit(i);
	}
	return tmp;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод 
{
	int temp;

	for (int i = 0; i < bf.BitLen; i++)
	{
		istr >> temp;

		if (temp == 1)
		{
			bf.SetBit(i);
		}
		else
		{
			bf.ClrBit(i);
		}
	}

	return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод 
{
	for (int i = 0; i < bf.BitLen; i++)
	{
		ostr << bf.GetBit(i) << ' ';
	}
	ostr << "\n";
	return ostr;
}