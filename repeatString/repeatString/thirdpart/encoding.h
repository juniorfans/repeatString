#ifndef TEST_ENCODING
#define TEST_ENCODING

typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;

/************************************************************************/
/* 
�������еı���ĺ���������з���ֵ�����������ֽ�ָ���ƫ��
�������ĺô��ǲ���ʹ�ö���ı����洢����������ռ�����ֽ�
*/
/************************************************************************/
//����̶����ȵ� 32 λ������ 64 λ����
extern void EncodeFixed32(char* dst, uint32_t value);
extern void EncodeFixed64(char* dst, uint64_t value);

//����̶����ȵ� 32 λ������ 64 λ����
extern uint32_t DecodeFixed32(const char* ptr,bool kLittleEndian);
extern uint64_t DecodeFixed64(const char* ptr,bool kLittleEndian);

//����䳤�� 32 λ������ 64 λ����
extern char* EncodeVarint32(char* dst, uint32_t value);
extern char* EncodeVarint64(char* dst, uint64_t value);

//����䳤�� 32 λ������ 64 λ����
extern const char* GetVarint32Ptr(const char* p,const char* limit,uint32_t* value) ;
extern const char* GetVarint64Ptr(const char* p, const char* limit, uint64_t* value);
// Internal routine for use by fallback path of GetVarint32Ptr
extern const char* GetVarint32PtrFallback(const char* p,
										  const char* limit,
										  uint32_t* value);

#endif