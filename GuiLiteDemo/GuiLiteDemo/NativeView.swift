//
//  NativeView.swift
//  GuiLiteDemo
//
//  Created by Zorro Young on 21/12/17.
//

import Cocoa

class NativeView: NSView {

    @IBOutlet var m_view: NSView!
    override func draw(_ dirtyRect: NSRect) {
        super.draw(dirtyRect)

        // Drawing code here.
        let img = buildImage(imgWidth: 1024, imgHeight: 768)
        let bg = NSColor.init(patternImage: img!)
        bg.setFill()
        NSRectFill(dirtyRect)
    }
    
    func buildImage(imgWidth: Int, imgHeight: Int) -> NSImage?{
        let cf_data = getPixels(imgWidth: imgWidth, imgHeight: imgHeight)
        let providerRef = CGDataProvider(data: cf_data!)
        let cgimg = CGImage.init(width: imgWidth, height: imgHeight, bitsPerComponent: 8, bitsPerPixel: 32, bytesPerRow: imgWidth*4, space: CGColorSpaceCreateDeviceRGB(), bitmapInfo: CGBitmapInfo.byteOrder32Big, provider: providerRef!, decode: nil, shouldInterpolate: true, intent: .defaultIntent)
        
        return NSImage(cgImage: cgimg!, size: CGSize(width: imgWidth, height: imgHeight))
    }
    
    func getPixels(imgWidth: Int, imgHeight: Int)->CFData?{
        let dataLength = imgWidth * imgHeight * 4
        let buffer = malloc(dataLength)
        
        for index in 0..<imgWidth*imgHeight {
            buffer?.storeBytes(of: 0x0000ff, toByteOffset: index*4, as: UInt32.self)
        }
        
        let rawPointer = UnsafeRawPointer(buffer)
        free(buffer)
        
        let pixData: UnsafePointer = rawPointer!.assumingMemoryBound(to: UInt8.self)
        return CFDataCreate(nil, pixData, dataLength)
    }
}
