//
//  ViewController.swift
//  GuiLiteDemo
//
//  Created by Zorro Young on 21/12/17.
//

import Cocoa

class ViewController: NSViewController {

    override func viewDidLoad() {
        super.viewDidLoad()

        // Do any additional setup after loading the view.
        Thread.detachNewThreadSelector(#selector(ViewController.run_native), toTarget: self, with: nil)
        
        let frame = CGRect(x: 0, y: 0, width: 1024, height: 768)
        let native_view = NativeView(frame: frame)
        view.addSubview(native_view)
    }

    override var representedObject: Any? {
        didSet {
        // Update the view, if already loaded.
        }
    }
    
    func run_native()
    {
        run_host_monitor();
    }
}
