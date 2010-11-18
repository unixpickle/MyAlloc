#import <Foundation/Foundation.h>
#import "ANCustomAllocation.h"

int main (int argc, const char * argv[]) {
    NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
	
	CFAllocatorRef alloc = AllocatorWithNewHeap();
	
	CFStringRef myString = CFStringCreateWithBytes(alloc, "Hello world!", 12, kCFStringEncodingASCII, FALSE);
	CFShow(myString);
	
	CFMutableArrayRef array = CFArrayCreateMutable(alloc, 3, NULL);
	CFArrayAppendValue(array, "Yo");
	
	CFRelease(array);
	CFRelease(myString);
	
	CFRelease(alloc);
	
    [pool drain];
    return 0;
}
