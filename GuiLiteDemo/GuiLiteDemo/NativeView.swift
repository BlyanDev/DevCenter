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
    
    override func mouseUp(with event: NSEvent) {
        
    }
    
    override func mouseDown(with event: NSEvent) {
        
    }
    
    func buildImage(imgWidth: Int, imgHeight: Int) -> NSImage?{
        let pixData = getPixels(imgWidth: imgWidth, imgHeight: imgHeight)
        let providerRef = CGDataProvider(data: pixData!)
        let cgimg = CGImage.init(width: imgWidth, height: imgHeight, bitsPerComponent: 8, bitsPerPixel: 32, bytesPerRow: imgWidth*4, space: CGColorSpaceCreateDeviceRGB(), bitmapInfo: CGBitmapInfo.byteOrder32Big, provider: providerRef!, decode: nil, shouldInterpolate: true, intent: .defaultIntent)
        
        return NSImage(cgImage: cgimg!, size: CGSize(width: imgWidth, height: imgHeight))
    }
    
    func getPixels(imgWidth: Int, imgHeight: Int)->CFData?{
        let dataLength = imgWidth * imgHeight * 4
        let buffer = malloc(dataLength)
        
        let rawData = get_frame_buffer(0, nil, nil)
        if rawData == nil{
            return nil
        }
        
        for index in 0..<imgWidth*imgHeight {
            let rgb16 = rawData?.load(fromByteOffset: index * 2, as: UInt16.self)
            let rgb32 = UInt32(rgb16!)
            let color = ((rgb32 << 3) & 0xFF) | (((rgb32) << 5) & 0xFF00) | ((rgb32 << 8) & 0xFF0000)
            buffer?.storeBytes(of: color, toByteOffset: index * 4, as: UInt32.self)
        }
        
        let rawPointer = UnsafeRawPointer(buffer)
        free(buffer)
        
        let pixData: UnsafePointer = rawPointer!.assumingMemoryBound(to: UInt8.self)
        return CFDataCreate(nil, pixData, dataLength)
    }
}
