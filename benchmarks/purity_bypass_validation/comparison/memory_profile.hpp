#pragma once
#ifdef ABW_MEMPROFILE
#include <cstdlib>
#include <cstddef>
#include <cstdint>
#include <new>
#include <malloc.h>
namespace abwmem {
struct alignas(std::max_align_t) H {void* raw; size_t size; uint64_t generation; bool aligned;};
inline uint64_t epoch=0;inline size_t live=0,peak=0,total=0,calls=0;inline bool enabled=false;
inline void* allocate(size_t n,size_t alignment=alignof(std::max_align_t),bool aligned=false){
 size_t extra=sizeof(H)+alignment;void* raw=aligned?_aligned_malloc(n+extra,alignment):std::malloc(n+extra);if(!raw)throw std::bad_alloc();
 auto p=(reinterpret_cast<uintptr_t>(raw)+sizeof(H)+alignment-1)&~uintptr_t(alignment-1);H*h=reinterpret_cast<H*>(p)-1;*h={raw,n,enabled?epoch:0,aligned};
 if(enabled){live+=n;peak=peak>live?peak:live;total+=n;calls++;}return reinterpret_cast<void*>(p);
}
inline void release(void*p)noexcept{if(!p)return;H*h=reinterpret_cast<H*>(p)-1;if(h->generation&&h->generation==epoch)live-=h->size;if(h->aligned)_aligned_free(h->raw);else std::free(h->raw);}
inline void begin(){++epoch;live=peak=total=calls=0;enabled=true;}inline void end(){enabled=false;}
}
void* operator new(size_t n){return abwmem::allocate(n);}void* operator new[](size_t n){return abwmem::allocate(n);}
void operator delete(void*p)noexcept{abwmem::release(p);}void operator delete[](void*p)noexcept{abwmem::release(p);}
void operator delete(void*p,size_t)noexcept{abwmem::release(p);}void operator delete[](void*p,size_t)noexcept{abwmem::release(p);}
void* operator new(size_t n,std::align_val_t a){return abwmem::allocate(n,size_t(a),true);}void* operator new[](size_t n,std::align_val_t a){return abwmem::allocate(n,size_t(a),true);}
void operator delete(void*p,std::align_val_t)noexcept{abwmem::release(p);}void operator delete[](void*p,std::align_val_t)noexcept{abwmem::release(p);}
void operator delete(void*p,size_t,std::align_val_t)noexcept{abwmem::release(p);}void operator delete[](void*p,size_t,std::align_val_t)noexcept{abwmem::release(p);}
inline void* abw_aligned_malloc(size_t n,size_t a){return abwmem::allocate(n,a,true);}inline void abw_aligned_free(void*p){abwmem::release(p);}
#endif
