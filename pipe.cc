#include <node.h>
#include <nan.h>

#if defined _WIN32 || defined _WIN64
   int pipe(int descf[2]) { return -1; }
   int dup(int fildes) { return -1; }
   int dup2(int fildes,int fields2) { return -1; }
#else
#  include <unistd.h>
#  include <fcntl.h>
#  include <errno.h>
   extern int errno;
#endif

#define ERRMSG "Could not create pipe"
#define DUPERRMSG "Could not create dup"


using namespace v8;


NAN_METHOD(syspipe) {
    Nan::HandleScope scope;

    int desc[2];
    char errmsg[128];

    if (pipe(desc)) {
#if defined _WIN32 || defined _WIN64
        return Nan::ThrowError(ERRMSG);
#else
        snprintf(errmsg, sizeof(errmsg), "%s (%s)", ERRMSG, strerror(errno));
        return Nan::ThrowError(errmsg);
#endif
   }

   Local<Object> obj = Nan::New<Object>();
   obj->Set(Nan::New<String>("read").ToLocalChecked(), Nan::New<Integer>(desc[0]));
   obj->Set(Nan::New<String>("write").ToLocalChecked(), Nan::New<Integer>(desc[1]));
   info.GetReturnValue().Set(obj);
}

NAN_METHOD(sysdup) {
    Nan::HandleScope scope;
    int fd_source= info[0]->Uint32Value();
    char errmsg[128];
    int fd_copy = fd_copy =dup(fd_source);
    if (fd_copy<0) {
#if defined _WIN32 || defined _WIN64
        return Nan::ThrowError(DUPERRMSG);
#else
        snprintf(errmsg, sizeof(errmsg), "%s (%s)", DUPERRMSG, strerror(errno));
        return Nan::ThrowError(errmsg);
#endif
   }

   Local<Object> obj = Nan::New<Object>();
   obj->Set(Nan::New<String>("source").ToLocalChecked(), Nan::New<Integer>(fd_source));
   obj->Set(Nan::New<String>("copy").ToLocalChecked(), Nan::New<Integer>(fd_copy));
   info.GetReturnValue().Set(obj);
}

void init(Handle<Object> exports) {
    exports->Set(Nan::New<String>("pipe").ToLocalChecked(),
            Nan::New<FunctionTemplate>(syspipe)->GetFunction());
    exports->Set(Nan::New<String>("dup").ToLocalChecked(),
            Nan::New<FunctionTemplate>(sysdup)->GetFunction());
}

NODE_MODULE(syspipe, init)

