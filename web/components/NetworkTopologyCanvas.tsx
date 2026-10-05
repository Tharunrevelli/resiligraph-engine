'use client';

import React from 'react';
import { Network, AlertOctagon, CheckCircle2, ShieldAlert, Anchor, Factory, MapPin } from 'lucide-react';

interface NetworkNodeView {
  id: number;
  name: string;
  country: string;
  type: 'SUPPLIER' | 'CHOKEPOINT' | 'RDC' | 'PLANT';
  capacity: string;
  isSevered: boolean;
  isSpof: boolean;
}

interface NetworkTopologyProps {
  nodes: NetworkNodeView[];
  onToggleSever: (id: number) => void;
}

export default function NetworkTopologyCanvas({ nodes, onToggleSever }: NetworkTopologyProps) {
  return (
    <div className="bg-white rounded-2xl border border-slate-200 shadow-sm p-6">
      <div className="flex flex-col sm:flex-row sm:items-center justify-between gap-4 pb-6 border-b border-slate-100">
        <div>
          <div className="flex items-center gap-2 text-amber-600 font-semibold text-xs uppercase tracking-wider">
            <Network className="w-4 h-4" /> Graph Analytics & SPOF Discovery
          </div>
          <h2 className="text-xl font-bold text-slate-900 mt-1">
            Global Logistics Network Flow & Chokepoint Map
          </h2>
          <p className="text-xs text-slate-500 mt-0.5">
            Click on any chokepoint or facility to simulate operational severance and trigger dynamic rerouting
          </p>
        </div>

        <div className="flex items-center gap-2">
          <span className="inline-flex items-center gap-1.5 px-3 py-1 rounded-full text-xs font-semibold bg-rose-50 text-rose-700 border border-rose-200">
            <AlertOctagon className="w-3.5 h-3.5" /> 3 Cut Vertices (Tarjan SPOF)
          </span>
        </div>
      </div>

      {/* Grid of Nodes categorized by echelon */}
      <div className="mt-6 space-y-6">
        {/* Tier 1 Suppliers */}
        <div>
          <span className="text-[11px] font-bold text-slate-400 uppercase tracking-wider">
            Tier-1 Global Suppliers (Sources)
          </span>
          <div className="grid grid-cols-1 sm:grid-cols-3 gap-3 mt-2">
            {nodes.filter((n) => n.type === 'SUPPLIER').map((n) => (
              <div
                key={n.id}
                onClick={() => onToggleSever(n.id)}
                className={`p-3.5 rounded-xl border cursor-pointer transition-all ${
                  n.isSevered
                    ? 'bg-rose-50 border-rose-400 shadow-sm'
                    : 'bg-slate-50 border-slate-200 hover:border-slate-300'
                }`}
              >
                <div className="flex justify-between items-start">
                  <div>
                    <span className="text-[10px] font-bold text-slate-400">Node #{n.id}</span>
                    <h4 className="text-xs font-bold text-slate-900 mt-0.5">{n.name}</h4>
                    <p className="text-[11px] text-slate-500">{n.country}</p>
                  </div>
                  {n.isSevered ? (
                    <span className="px-2 py-0.5 rounded text-[10px] font-bold bg-rose-200 text-rose-800">SEVERED</span>
                  ) : (
                    <span className="px-2 py-0.5 rounded text-[10px] font-bold bg-emerald-100 text-emerald-800">ONLINE</span>
                  )}
                </div>
                <div className="mt-2 pt-2 border-t border-slate-200/60 flex justify-between text-[11px] text-slate-600">
                  <span>Capacity: {n.capacity}</span>
                  {n.isSpof && <span className="font-bold text-amber-700">★ SPOF Cut Vertex</span>}
                </div>
              </div>
            ))}
          </div>
        </div>

        {/* Maritime / Overland Chokepoints */}
        <div>
          <span className="text-[11px] font-bold text-slate-400 uppercase tracking-wider">
            Maritime Corridors & Port Chokepoints
          </span>
          <div className="grid grid-cols-1 sm:grid-cols-3 gap-3 mt-2">
            {nodes.filter((n) => n.type === 'CHOKEPOINT').map((n) => (
              <div
                key={n.id}
                onClick={() => onToggleSever(n.id)}
                className={`p-3.5 rounded-xl border cursor-pointer transition-all ${
                  n.isSevered
                    ? 'bg-rose-50 border-rose-400 shadow-sm'
                    : n.isSpof
                    ? 'bg-amber-50/40 border-amber-300 hover:border-amber-400'
                    : 'bg-slate-50 border-slate-200 hover:border-slate-300'
                }`}
              >
                <div className="flex justify-between items-start">
                  <div>
                    <span className="text-[10px] font-bold text-slate-400">Node #{n.id}</span>
                    <h4 className="text-xs font-bold text-slate-900 mt-0.5 flex items-center gap-1">
                      <Anchor className="w-3.5 h-3.5 text-blue-600" /> {n.name}
                    </h4>
                    <p className="text-[11px] text-slate-500">{n.country}</p>
                  </div>
                  {n.isSevered ? (
                    <span className="px-2 py-0.5 rounded text-[10px] font-bold bg-rose-200 text-rose-800">SEVERED</span>
                  ) : (
                    <span className="px-2 py-0.5 rounded text-[10px] font-bold bg-blue-100 text-blue-800">ONLINE</span>
                  )}
                </div>
                <div className="mt-2 pt-2 border-t border-slate-200/60 flex justify-between text-[11px] text-slate-600">
                  <span>Capacity: {n.capacity}</span>
                  {n.isSpof && <span className="font-bold text-amber-700">★ Critical Chokepoint</span>}
                </div>
              </div>
            ))}
          </div>
        </div>

        {/* Regional DCs & Plant */}
        <div>
          <span className="text-[11px] font-bold text-slate-400 uppercase tracking-wider">
            Inland Distribution Centers & Final Assembly Plant (Sink)
          </span>
          <div className="grid grid-cols-1 sm:grid-cols-3 gap-3 mt-2">
            {nodes.filter((n) => n.type === 'RDC' || n.type === 'PLANT').map((n) => (
              <div
                key={n.id}
                onClick={() => n.type !== 'PLANT' && onToggleSever(n.id)}
                className={`p-3.5 rounded-xl border transition-all ${
                  n.type === 'PLANT'
                    ? 'bg-indigo-50/60 border-indigo-200 cursor-default'
                    : n.isSevered
                    ? 'bg-rose-50 border-rose-400 cursor-pointer shadow-sm'
                    : 'bg-slate-50 border-slate-200 hover:border-slate-300 cursor-pointer'
                }`}
              >
                <div className="flex justify-between items-start">
                  <div>
                    <span className="text-[10px] font-bold text-slate-400">Node #{n.id}</span>
                    <h4 className="text-xs font-bold text-slate-900 mt-0.5 flex items-center gap-1">
                      {n.type === 'PLANT' ? <Factory className="w-3.5 h-3.5 text-indigo-600" /> : <MapPin className="w-3.5 h-3.5 text-slate-600" />}
                      {n.name}
                    </h4>
                    <p className="text-[11px] text-slate-500">{n.country}</p>
                  </div>
                  {n.isSevered ? (
                    <span className="px-2 py-0.5 rounded text-[10px] font-bold bg-rose-200 text-rose-800">SEVERED</span>
                  ) : (
                    <span className="px-2 py-0.5 rounded text-[10px] font-bold bg-emerald-100 text-emerald-800">OPERATIONAL</span>
                  )}
                </div>
                <div className="mt-2 pt-2 border-t border-slate-200/60 flex justify-between text-[11px] text-slate-600">
                  <span>Capacity: {n.capacity}</span>
                  {n.isSpof && <span className="font-bold text-amber-700">★ High Impact RDC</span>}
                </div>
              </div>
            ))}
          </div>
        </div>
      </div>
    </div>
  );
}
