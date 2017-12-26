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
        
        //let path = NSBezierPath()
        //path.move(to: CGPoint(x: 0 , y: 0))
        //path.line(to: CGPoint(x: 100 , y: 100))
        //path.stroke()
        
        //NSColor.red.setFill()
        //NSRectFill(dirtyRect)
        
        //let img = NSImage(named: "flip.png")
        let img = buildImage(imgWidth: 100, imgHeight: 100)
        let bg = NSColor.init(patternImage: img!)
        bg.setFill()
        NSRectFill(dirtyRect)
    }
    
    func buildImage(imgWidth: Int, imgHeight: Int) -> NSImage?{
        let cf_data = getPixels(imgWidth: imgWidth, imgHeight: imgHeight)
        
        //let providerRef = CGDataProvider.init(data: cf_data!)
        let providerRef = CGDataProvider(data: cf_data!)
        
        //let bitmapInfo = CGBitmapInfo(rawValue: CGImageAlphaInfo.premultipliedFirst.rawValue)
        let bitmapInfo = CGBitmapInfo.byteOrder32Big
        
        let colorSpace = CGColorSpaceCreateDeviceRGB()
        
        let cgimg = CGImage.init(width: imgWidth, height: imgHeight, bitsPerComponent: 8, bitsPerPixel: 32, bytesPerRow: imgWidth*4, space: colorSpace, bitmapInfo: bitmapInfo, provider: providerRef!, decode: nil, shouldInterpolate: true, intent: .defaultIntent)
        
        return NSImage(cgImage: cgimg!, size: CGSize(width: imgWidth, height: imgHeight))
    }
    
    func getPixels(imgWidth: Int, imgHeight: Int)->CFData?{
        let dataLength = imgWidth * imgHeight * 4
        let buffer = malloc(dataLength)
        let rawPointer = UnsafeRawPointer(buffer)
        let pixData: UnsafePointer = rawPointer!.assumingMemoryBound(to: UInt8.self)
        //let cf_data = CFDataCreate(kCFAllocatorDefault, pixData, dataLength)
        return CFDataCreate(nil, pixData, dataLength)
    }
}
