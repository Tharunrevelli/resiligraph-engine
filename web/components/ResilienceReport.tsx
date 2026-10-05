'use client';

import React from 'react';
import { GitCompare, ShieldCheck, DollarSign, TrendingUp, CheckCircle2 } from 'lucide-react';

export default function ResilienceReport() {
  const recommendations = [
    {
      title: 'Port Chokepoint Dual-Routing Contract',
      risk: 'Over-reliance on Port of Singapore exposes 45% of Detroit factory throughput to regional maritime strikes.',
      toBuy: 'Establish pre-negotiated capacity reservations through Port of Rotterdam and direct rail intermodal links.',
      roi: 'Mitigates potential $12.5M in plant idle-time losses per week of disruption.',
    },
    {
      title: 'Inland Regional DC Redundancy',
      risk: 'Chicago RDC is an identified Tarjan articulation cut vertex handling 70% of inbound Detroit components.',
      toBuy: 'Activate secondary multi-modal bypass hub in Columbus, OH with synchronized (s, S) buffer stocks.',
      roi: 'Reduces systemic supply chain vulnerability index from 30.0% to 11.2%.',
    },
    {
      title: 'Real-Time C++ Dijkstra Flow Rerouting',
      risk: 'Manual carrier renegotiation during sudden chokepoints takes 3 to 5 business days.',
      toBuy: 'Embed sub-millisecond C++ network optimization into enterprise ERP/TMS dispatch systems.',
      roi: 'Automated instant rerouting saves an average of $380/TEU in emergency spot-freight surcharges.',
    },
  ];

  return (
    <div className="bg-white rounded-2xl border border-slate-200 shadow-sm p-6">
      <div className="flex flex-col sm:flex-row sm:items-center justify-between gap-4 pb-6 border-b border-slate-100">
        <div>
          <div className="flex items-center gap-2 text-amber-600 font-semibold text-xs uppercase tracking-wider">
            <GitCompare className="w-4 h-4" /> Strategic Consulting Roadmap
          </div>
          <h2 className="text-xl font-bold text-slate-900 mt-1">
            Supply Chain Vulnerability & Network Hardening Strategy
          </h2>
          <p className="text-xs text-slate-500 mt-0.5">
            Accenture S&C Executive Point of View (PoV) & Enterprise Resilience Recommendations
          </p>
        </div>

        <div className="flex items-center gap-2">
          <span className="inline-flex items-center gap-1.5 px-3 py-1 rounded-full text-xs font-semibold bg-emerald-50 text-emerald-700 border border-emerald-200">
            <CheckCircle2 className="w-3.5 h-3.5" /> High-Value ROI
          </span>
        </div>
      </div>

      <div className="grid grid-cols-1 md:grid-cols-3 gap-4 mt-6">
        {recommendations.map((rec, idx) => (
          <div key={idx} className="bg-slate-50/60 p-4 rounded-xl border border-slate-200 flex flex-col justify-between">
            <div>
              <span className="text-[10px] font-bold text-amber-700 uppercase tracking-wider">Strategic Action #{idx + 1}</span>
              <h4 className="text-sm font-bold text-slate-900 mt-1">{rec.title}</h4>
              <p className="text-xs text-slate-500 mt-2"><strong className="text-slate-700">Vulnerability:</strong> {rec.risk}</p>
              <p className="text-xs text-slate-600 mt-2"><strong className="text-slate-800">Target To-Be Architecture:</strong> {rec.toBuy}</p>
            </div>
            <div className="mt-4 pt-3 border-t border-slate-200 text-xs font-semibold text-emerald-700 flex items-start gap-1">
              <TrendingUp className="w-4 h-4 shrink-0 mt-0.5 text-emerald-600" />
              <span>{rec.roi}</span>
            </div>
          </div>
        ))}
      </div>
    </div>
  );
}
